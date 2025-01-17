/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/17 10:05:15 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include "tokenizer.h"
#include "ast.h"
#include "expansion.h"
#include "here_document.h"
#include <readline/history.h>
#include <readline/readline.h>
#include "execution.h"

void	print_field(void *field_ptr)
{
	t_field	*field;


	field = field_ptr;

	printf("{%s, ", field->value->data);
#if 0
	size_t	i=0;
	while (i < field->mask->size)
	{ 	
		char c = 'O';
		char q = ' ';
		if (field->mask->data[i] & EXPANDED)
			c = 'E';

		if (field->mask->data[i] & SINGLE_QUOTED)
			q = 'S';
		else if (field->mask->data[i] & DOUBLE_QUOTED)
			q = 'D';
		printf("<%c%c>", c, q);
		i++;
	}
#endif
	printf("}");
	fflush(NULL);
}

void	print_token(void *token_ptr)
{
	t_array	*lexems;
	t_token	*token;
	char	*id;
	lexems = lexems_get_instance();
	token = token_ptr;
	if (token == NULL)
	{
		printf("(null)");
		return ;
	}
	if (token->type == 0)
		id = "WORD";
	else if(token->type == -1)
		id = "EOF";
	else
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	printf("[%s, ", id);
	fflush(NULL);
	array_do(token->fields, print_field);
	printf("]");
}

void	print_redirection(void *tokens_ptr)
{
	//t_array *tokens;

	//tokens = tokens_ptr;
	printf("{");
//	array_do(tokens, print_token);
	print_token(tokens_ptr);
	printf("}");
}

static void	print(void *node_ptr);

static void print_children(t_array *children, char *symb)
{
	printf("%s ", symb);
	array_do(children, print);
}

static void	print(void *node_ptr)
{
	t_ast_node	*node;

	node = node_ptr;
	if(node == NULL)
		return;
	if (node->type == AST_COMPLETE_COMMAND)
	{
		print_children(node->children, "Complete: ");
		printf("%s\n", node->error_message);
	}
	else if (node->type == AST_COMMAND_LIST)
		print_children(node->children, "List: ");
	else if (node->type == AST_COMPOUND_COMMAND)
		print_children(node->children, "Compound: ");
	else if (node->type == AST_BINARY_AND)
		printf("\n\tAND: ");
	else if (node->type == AST_BINARY_OR)
		printf("\n\tOR: ");
	else if (node->type == AST_PIPELINE)
		print_children(node->children, "\n\t\tPipeline: ");
	else if (node->type == AST_COMMAND)
		print_children(node->children, "Command: ");
	else if (node->type == AST_SUBSHELL)
	{
		print_children(node->children, "\n\t\t\tsubshell: ");
		if (node->redirect_list)
		{
			printf("\n\t\t\t\tsubshell_redirection: ");
			array_do(node->redirect_list, print_redirection);
		}
	}
	else if (node->type == AST_SIMPLE_COMMAND)
	{
		printf("\n\t\t\tSimple_command: ");
		array_do(node->children, print_token);
		array_do(node->redirect_list, print_redirection);
	}
}

enum
{
	NO_EXPANSIONS = 0,
	PARAMETER_EXPANSION = 1,
	FIELD_SPLITTING = 2,
	PATHNAME_EXPANSION = 4,
	QUOTE_REMOVAL = 8,
	ALL_EXPANSIONS = 15,
};

bool	is_export_command(t_token *token)
{
	return (ft_strcmp("export", token->value) == 0);
}

bool	is_assingment_word(t_token *token)
{
	//TODO: check if the token has this form: key=value or key+=value where key is a valid variable name
	char *key;

	key = get_key(token->value);
	if (key == NULL)
		return (false);
	free(key);
	return (true);
}

void	expand_token(t_token *token, int flag)
{
	if (flag & PARAMETER_EXPANSION)
		parameter_expansion(token);
	if (flag & FIELD_SPLITTING)
		field_splitting(token);
	if (flag & PATHNAME_EXPANSION)
		pathname_expansion(token);
	if (flag & QUOTE_REMOVAL)
		quote_removal(token);
}

void	expansion(t_array *tokens)
{
	t_token	*token;
	int	flag;
	bool	is_export;
	size_t	i;

	i = 0;
	if (tokens->size == 0)
		return ;
	is_export = false;
	while (i < tokens->size)
	{
		token = array_get(tokens, i);
		flag = ALL_EXPANSIONS;
		if (token->type == lexem_get_type("HERE_DOCUMENT"))
			flag = NO_EXPANSIONS;
		else if (token->type == lexem_get_type("WORD") && is_export && is_assingment_word(token))
			flag = PARAMETER_EXPANSION | QUOTE_REMOVAL;
		expand_token(token, flag);
		is_export = is_export_command(token);
		i++;
	}
}

static void	handle_expansions(void *node_ptr)
{
	t_ast_node *node;

	node = node_ptr;
	if (node->children == NULL)
		return ;
	if (node->type != AST_SIMPLE_COMMAND && node->type != AST_SUBSHELL)
	{
		 array_do(node->children, handle_expansions);
		 return ;
	}
	if (node->type == AST_SUBSHELL)
		array_do(node->children, handle_expansions);
	if (node->type == AST_SIMPLE_COMMAND)
		expansion(node->children);
	expansion(node->redirect_list);
	array_do(node->redirect_list, handle_heredoc);
}

int	main()
{
	char		*line;
	t_string	input;
	t_array		*tokens;
	t_array		*list;

	get_environment_instance();
	string_init(&input);

	if (isatty(0) == 0)
	{
		line = get_next_line(0);
		if (ft_strchr(line, '\n'))
			*ft_strchr(line, '\n') = '\0';
		string_set(&input, line);
		tokens = tokenize(&input);
		list = generate_ast(tokens);
	//	array_do(list, handle_expansions);
		int status = execution(list);
		manager_free_everything();
		exit(status);
	}
	while (1)
	{
		line = readline(prompt());
		if (line == NULL || ft_strlen(line) == 0)
		{
			free(line);
			continue;
		}
		add_history(line);
		string_set(&input, line);
		tokens = tokenize(&input);
		if (tokens == NULL)
			continue ;
		list = generate_ast(tokens);
		array_do(list, handle_expansions);
		//array_do(list, print);
		execution(list);
		free(line);
	}
	rl_clear_history();
	manager_free_everything();
}

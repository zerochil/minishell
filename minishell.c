/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/14 09:38:53 by inajah           ###   ########.fr       */
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

	printf("{%s}", field->value->data);
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
	
	array_do(node->children, parameter_expansion);
	array_do(node->children, field_splitting);
	array_do(node->children, pathname_expansion);
	array_do(node->children, quote_removal);
	//array_do(node->redirect_list, handle_heredoc);
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
		line = readline("minishell> ");
		add_history(line);
		string_set(&input, line);
		tokens = tokenize(&input);
		if (tokens == NULL)
			continue ;
		list = generate_ast(tokens);
		array_do(list, handle_expansions);
		array_do(list, print);
		//execution(list);
		free(line);
	}
	rl_clear_history();
	manager_free_everything();
}

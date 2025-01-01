/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/01 08:47:59 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include "tokenizer.h"
#include "ast.h"
#include <readline/history.h>
#include <readline/readline.h>

void	print_token(void *token_ptr)
{
	t_array	*lexems;
	t_token	*token;
	char	*id;
	lexems = lexems_get_instance();
	token = token_ptr;
	if (token->type == 0)
		id = "WORD";
	else if(token->type == -1)
		id = "EOF";
	else
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	printf("[%s, %s]", id, token->filename);
	fflush(NULL);
}

static void	print(void *node_ptr);

static void print_children(t_array *children, char *symb)
{
	printf("%s ", symb);
	array_do(children, print);
	/*printf(" %c", symb[1]);*/
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
			array_do(node->redirect_list, print_token);
		}
	}
	else if (node->type == AST_SIMPLE_COMMAND)
	{
		printf("\n\t\t\tSimple_command: ");
		array_do(node->children, print_token);
	}
}

int	main(void)
{
	char		*line;
	t_string	input;
	t_array		*tokens;
	t_array		*list;

	string_init(&input);
	line = "ls -a 'xs'>> xsx\nls && sl | wi\n ls && ls \n ls | ls \nwc | cat > helo >> world";
	/*line = "ls\n()\ncat | wc";*/
	/*line = "ls | lx";*/
	/*printf("%s\n", line);*/
	/*string_set(&input, line);*/
	/*tokens = tokenize(&input);*/
	/*t_ast_node *list = complete_command(tokens);*/
	/*if (list)*/
	/*	print(list);*/
	/*else*/
	/*	printf("error\n");*/
	/**/
		string_set(&input, line);
		tokens = tokenize(&input);
		printf("minishell: %s\n", line);
		//array_do(tokens, print_token);
		printf("\n");
		list = generate_ast(tokens);
		if (list->size > 0 )
			array_do(list, print);
		else {
			printf("error\n");
		}
	/*while (1)*/
	/*{*/
	/*	line = readline("minishell> ");*/
	/*	add_history(line);*/
	/*	string_set(&input, line);*/
	/*	tokens = tokenize(&input);*/
	/*	array_do(tokens, print_token);*/
	/*	list = complete_command(tokens);*/
	/*	if (list != NULL)*/
	/*		print(list);*/
	/*}*/
	rl_clear_history();
	manager_free_everything();
}

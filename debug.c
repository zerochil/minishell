/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 09:28:43 by inajah            #+#    #+#             */
/*   Updated: 2025/01/25 09:18:29 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	print_vars(void *str_ptr)
{
	char *str;

	str = str_ptr;
	if (str)
		printf("%s\n", str);
}


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

static void	print_redirection(void *tokens_ptr)
{
	//t_array *tokens;

	//tokens = tokens_ptr;
	printf("{");
//	array_do(tokens, print_token);
	print_token(tokens_ptr);
	printf("}");
}

static void print_children(t_array *children, char *symb)
{
	printf("%s ", symb);
	array_do(children, print);
}

void	print(void *node_ptr)
{
	t_ast_node	*node;

	node = node_ptr;
	if(node == NULL)
		return;
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

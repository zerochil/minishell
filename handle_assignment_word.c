/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_assignment_word.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:07 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/07 18:16:39 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static void assign_assignemnt_word(t_array *tokens)
{
	t_token *token;
	char *word;
	char *value;

	token = token_ptr;
	word = token->value->data;
	value = ft_strchr(word, '=');
	if (value == NULL)
		return ;
	*value = '\0';
	setenv(word, value + 1, 1);
}

void handle_assignment_word(void *node_ptr)
{
	t_ast_node *node;

	node = node_ptr;
	if (node->type == AST_SIMPLE_COMMAND)
		assign_assignemnt_word(node->children);;
	else
		array_do(node->children, handle_assignment_word);
}

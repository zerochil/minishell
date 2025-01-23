/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 08:42:34 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 08:42:34 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ast.h>

bool	skip_linebreak(t_array *tokens)
{
	t_token	*token;
	bool	found;

	found = false;
	while (true)
	{
		token = array_peek(tokens);
		if (token->type != lexem_get_type("NEWLINE"))
			break ;
		array_shift(tokens);
		found = true;
	}
	return (found);
}

int	is_logical_and_or(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("LOGIC_AND"))
		return (AST_BINARY_AND);
	if (token->type == lexem_get_type("LOGIC_OR"))
		return (AST_BINARY_OR);
	return (-1);
}

t_ast_node	*create_ast_node(t_array *children, t_ast_type type)
{
	t_ast_node	*node;

	node = track_malloc(sizeof(t_ast_node));
	node->type = type;
	node->children = children;
	node->redirect_list = NULL;
	node->error_message = "";
	return (node);
}

t_array	*consume_until(t_array *tokens, bool is_target(int), bool to_skip(int),
		bool is_error(t_token *))
{
	t_array	*consumed;
	t_token	*token;
	size_t	index;

	consumed = array_create();
	index = 0;
	while (index < tokens->size)
	{
		token = array_get(tokens, index);
		if (token == NULL)
			break ;
		if (is_target(token->type))
		{
			token = array_remove(tokens, index);
			if (is_error && is_error(token))
				return (NULL);
			array_push(consumed, token);
		}
		else if (to_skip && to_skip(token->type))
			index++;
		else
			break ;
	}
	return (consumed);
}

bool	is_filename_missing(t_token *token)
{
	if (token->value == NULL)
	{
		syntax_error(ERR_MISSING_FILENAME);
		return (true);
	}
	return (false);
}

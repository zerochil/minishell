/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 08:43:32 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 08:45:41 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ast.h>
#include <tokenizer.h>

static bool	is_compound_terminated(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == LEXEM_EOF || token->type == lexem_get_type("NEWLINE"));
}

static bool	check_compound_termination(t_ast_node *compound_node,
		t_array *tokens)
{
	if (compound_node != NULL && is_compound_terminated(tokens))
		return (true);
	return (false);
}

static void	push_error_node(t_array *ast_list, t_array *tokens)
{
	t_ast_node	*error_node;
	char		*error_message;

	error_node = create_ast_node(NULL, AST_INVALID_COMMAND);
	error_message = syntax_error("");
	if (error_message == NULL)
		error_message = syntax_error("syntax error near unexpected token");
	error_node->error_message = error_message;
	array_push(ast_list, error_node);
	while (is_compound_terminated(tokens) == false)
		array_shift(tokens);
}

static bool	is_eof(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == LEXEM_EOF);
}

t_array	*generate_ast(t_array *tokens)
{
	t_array		*compound_list;
	t_ast_node	*compound_node;

	compound_list = array_create();
	skip_linebreak(tokens);
	while (is_eof(tokens) == false)
	{
		syntax_error(NULL);
		compound_node = compound_command(tokens);
		if (check_compound_termination(compound_node, tokens) == false)
			push_error_node(compound_list, tokens);
		else
			array_push(compound_list, compound_node);
		if (skip_linebreak(tokens) == false)
			break ;
	}
	return (compound_list);
}

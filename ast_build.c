/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 08:38:41 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 08:38:42 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ast.h>

t_ast_node	*compound_command(t_array *tokens)
{
	t_ast_node	*pipes;
	t_array		*compounds;
	int			type;

	compounds = array_create();
	while (true)
	{
		pipes = pipeline(tokens);
		if (pipes == NULL)
			return (NULL);
		array_push(compounds, pipes);
		type = is_logical_and_or(tokens);
		if (type != -1)
		{
			array_push(compounds, create_ast_node(NULL, type));
			array_shift(tokens);
			skip_linebreak(tokens);
		}
		else
			break ;
	}
	return (create_ast_node(compounds, AST_COMPOUND_COMMAND));
}

t_ast_node	*pipeline(t_array *tokens)
{
	t_token		*token;
	t_array		*pipes;
	t_ast_node	*command_node;

	pipes = array_create();
	while (true)
	{
		command_node = command(tokens);
		if (command_node == NULL)
			return (NULL);
		array_push(pipes, command_node);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("PIPE"))
			array_shift(tokens);
		else
			break ;
		skip_linebreak(tokens);
	}
	return (create_ast_node(pipes, AST_PIPELINE));
}

t_ast_node	*command(t_array *tokens)
{
	t_token		*token;
	t_ast_node	*subshell_node;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("OPEN_PARENTHESIS"))
	{
		array_shift(tokens);
		subshell_node = subshell(tokens);
		if (subshell_node->children->size == 0)
			return (syntax_error(ERR_EMPTY_COMMAND), NULL);
		return (subshell_node);
	}
	return (simple_command(tokens));
}

t_ast_node	*subshell(t_array *tokens)
{
	t_array		*compound_list;
	t_ast_node	*compound_node;
	t_token		*token;
	t_ast_node	*subshell_node;

	compound_list = array_create();
	while (true)
	{
		skip_linebreak(tokens);
		compound_node = compound_command(tokens);
		if (compound_node == NULL)
			return (NULL);
		array_push(compound_list, compound_node);
		skip_linebreak(tokens);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			array_shift(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			break ;
	}
	subshell_node = create_ast_node(compound_list, AST_SUBSHELL);
	subshell_node->redirect_list = consume_until(tokens, lexem_is_redirection,
			NULL, is_filename_missing);
	return (subshell_node);
}

t_ast_node	*simple_command(t_array *tokens)
{
	t_ast_node	*simple_command_node;
	t_array		*argument_list;
	t_array		*redirections;

	argument_list = consume_until(tokens, lexem_is_word, lexem_is_redirection,
			NULL);
	redirections = consume_until(tokens, lexem_is_redirection, lexem_is_word,
			is_filename_missing);
	if (redirections == NULL || argument_list == NULL)
		return (NULL);
	if (check_syntax_error(argument_list->size == 0 && redirections->size == 0,
			ERR_WORD_OR_REDIR))
		return (NULL);
	simple_command_node = create_ast_node(argument_list, AST_SIMPLE_COMMAND);
	simple_command_node->redirect_list = redirections;
	return (simple_command_node);
}

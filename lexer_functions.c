/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:12:37 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:45:47 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

int	lexem_match_identifier(void *lexem_ptr, void *identifier)
{
	t_lexem	*lexem;

	lexem = lexem_ptr;
	return (ft_strncmp(lexem->identifier, identifier,
			lexem->identifier_length) == 0);
}

int	lexem_match_symbol(void *lexem_ptr, void *input_ptr)
{
	t_lexem		*lexem;
	t_string	*input;

	lexem = lexem_ptr;
	input = input_ptr;
	return (string_match(input, lexem->symbol, ft_strncmp, 0));
}

int	lexem_match_word_break(void *lexem_ptr, void *input_ptr)
{
	t_lexem		*lexem;
	t_string	*input;

	lexem = lexem_ptr;
	input = input_ptr;
	return (string_match(input, lexem->symbol, ft_strncmp, input->peek));
}

bool	lexem_is_redirection(int type)
{
	return (type == lexem_get_type("REDIRECTION_IN")
		|| type == lexem_get_type("REDIRECTION_APPEND")
		|| type == lexem_get_type("REDIRECTION_TRUNC")
		|| type == lexem_get_type("HERE_DOCUMENT"));
}

bool	lexem_is_word(int type)
{
	return (type == lexem_get_type("WORD"));
}

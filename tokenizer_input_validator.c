/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_input_validator.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:54:38 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 18:00:30 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	is_quoted(t_string *input)
{
	char	in_quote;
	char	c;

	in_quote = '\0';
	while (1)
	{
		c = string_peek_advance(input);
		if (c == '\0')
			break ;
		if (in_quote != '\'' && c == '\\')
			string_peek_advance(input);
		else if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
	}
	string_peek_reset(input);
	return (in_quote == '\0');
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:54:38 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 09:18:40 by rrochd           ###   ########.fr       */
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

t_token	*token_init(int type, char *value)
{
	t_token	*token;

	token = track_malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->fields = NULL;
	if (value)
	{
		token->fields = track_malloc(sizeof(t_array));
		array_init(token->fields);
		array_push(token->fields, field_init(value, NULL));
	}
	return (token);
}

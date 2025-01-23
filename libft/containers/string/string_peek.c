/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_peek.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:32:58 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:37:28 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char	string_peek(t_string *string)
{
	if (string == NULL)
	{
		report_error("string_peek: error");
		return (0);
	}
	return (string->data[string->peek]);
}

char	string_peek_reset(t_string *string)
{
	if (string == NULL)
	{
		report_error("string_peek_reset: error");
		return (0);
	}
	string->peek = 0;
	return (string->data[string->peek]);
}

char	string_peek_advance(t_string *string)
{
	char	last_character;

	if (string == NULL)
	{
		report_error("string_peek_advance: error");
		return (0);
	}
	last_character = string->data[string->peek];
	if (string->peek + 1 <= string->size)
		string->peek++;
	return (last_character);
}

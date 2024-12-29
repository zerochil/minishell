/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:00:35 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/22 18:44:59 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char	string_shift(t_string *string)
{
	char	c;

	if (string == NULL || string->size == 0)
		return (-1); // TODO: AGAIN; am not sure about the return value here;
	c = string->data[0];
	ft_memmove(string->data, string->data + 1, string->size - 1);
	string->size--;
	string->data[string->size] = '\0';
	return c;
}

void	string_append(t_string *string, const char *text)
{
	size_t	text_len;

	if (string == NULL || text == NULL)
		return ;
	text_len = ft_strlen(text);
	string_ensure_capacity(string, string->size + text_len);
	ft_memcpy(string->data + string->size, text, text_len);
	string->size += text_len;
}

void	string_insert(t_string *string, const char *text, size_t index)
{
	size_t	text_len;

	if (string == NULL || text == NULL || index > string->size)
		return ;
	text_len = ft_strlen(text);
	string_ensure_capacity(string, string->size + text_len);
	ft_memmove(
		string->data + index + text_len,
		string->data + index,
		string->size - index
		);
	ft_memcpy(string->data + index, text, text_len);
	string->size += text_len;
	string->data[string->size] = '\0';
}

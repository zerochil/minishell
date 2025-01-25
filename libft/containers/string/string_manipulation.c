/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:00:35 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/05 10:32:00 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char	string_shift(t_string *string)
{
	char	c;

	if (string == NULL || string->size == 0)
	{
		report_error("string_shift: error");
		return (0);
	}
	c = string->data[0];
	ft_memmove(string->data, string->data + 1, string->size - 1);
	string->size--;
	string->data[string->size] = '\0';
	return (c);
}

void	string_shift_by(t_string *string, size_t offset)
{
	if (string == NULL || string->size == 0)
	{
		report_error("string_shift_by: error");
		return ;
	}
	if (offset > string->size)
		offset = string->size;
	ft_memmove(string->data, string->data + offset, string->size - offset);
	string->size -= offset;
	string->data[string->size] = '\0';
}

void	string_shift_while(t_string *string, char *charset)
{
	if (string == NULL || charset == NULL)
	{
		report_error("string_shift_while: error");
		return ;
	}
	while (string->size > 0 && ft_strchr(charset, string->data[0]))
		string_shift(string);
}

void	string_append(t_string *string, const char *text)
{
	size_t	text_len;

	if (text == NULL)
		return ;
	if (string == NULL)
	{
		report_error("string_append: error");
		return ;
	}
	text_len = ft_strlen(text);
	string_ensure_capacity(string, string->size + text_len);
	ft_memcpy(string->data + string->size, text, text_len + 1);
	string->size += text_len;
}

void	string_insert(t_string *string, const char *text, size_t index)
{
	size_t	text_len;

	if (string == NULL || text == NULL || index > string->size)
	{
		report_error("string_insert: error");
		return ;
	}
	text_len = ft_strlen(text);
	string_ensure_capacity(string, string->size + text_len);
	ft_memmove(string->data + index + text_len, string->data + index,
		string->size - index);
	ft_memcpy(string->data + index, text, text_len);
	string->size += text_len;
	string->data[string->size] = '\0';
}

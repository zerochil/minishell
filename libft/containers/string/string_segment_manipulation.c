/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_segment_manipulation.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 02:47:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/19 18:53:10 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

void	string_segment_remove(t_string *string, size_t start, size_t length)
{
	if (string == NULL || string->size == 0 || start >= string->size)
	{
		report_error("string_segment_remove: error");
		return ;
	}
	if (length == 0)
		return ;
	if (start + length > string->size)
		length = string->size - start;
	ft_memmove(string->data + start, string->data + start + length, string->size
		- (start + length));
	string->size -= length;
	string->data[string->size] = '\0';
}

char	*string_segment_extract(t_string *string, size_t start, size_t length)
{
	char	*result;

	if (string == NULL || start >= string->size)
	{
		report_error("string_segment_extract: error");
		return (NULL);
	}
	if (start + length > string->size)
		length = string->size - start;
	result = track_malloc((length + 1) * sizeof(char));
	ft_memcpy(result, string->data + start, length);
	result[length] = '\0';
	return (result);
}

void	string_segment_replace(t_string *string, size_t start, size_t length,
		const char *new_text)
{
	if (length == 0)
		return ;
	string_segment_remove(string, start, length);
	string_insert(string, new_text, start);
}

char	*string_segment_slice(t_string *string, size_t start, size_t length)
{
	char	*result;

	if (length == 0)
		return (NULL);
	result = string_segment_extract(string, start, length);
	string_segment_remove(string, start, length);
	return (result);
}

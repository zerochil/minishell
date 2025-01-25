/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:32:58 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:42:39 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char	string_at(t_string *string, size_t index)
{
	if (string == NULL || index > string->size)
	{
		report_error("string_at: error");
		return (-1);
	}
	return (string->data[index]);
}

int	string_match(t_string *string, char *target, int (*cmp)(const char *,
			const char *, size_t), size_t start)
{
	if (string == NULL || start > string->size)
	{
		report_error("string_match: error");
		return (-1);
	}
	return (cmp(string->data + start, target, ft_strlen(target)) == 0);
}

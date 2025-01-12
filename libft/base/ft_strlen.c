/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:02:24 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/21 20:29:36 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

size_t	ft_strlen(const char *s)
{
	size_t	length;

	if (s == NULL)
		return (0);
	length = 0;
	while (s[length])
		length++;
	return (length);
}

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	length;

	if (s == NULL)
		return (0);
	length = 0;
	while (s[length] && length < maxlen)
		length++;
	return (length);
}

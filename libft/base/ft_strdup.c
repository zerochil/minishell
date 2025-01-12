/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 01:54:07 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:42 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*string;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1) + 1;
	string = malloc(len);
	if (!string)
		return (NULL);
	ft_memcpy(string, s1, len);
	return (string);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	char	*string;

	if (s1 == NULL)
		return (NULL);
	len = ft_strnlen(s1, n);
	string = malloc(len + 1);
	if (!string)
		return (NULL);
	ft_memcpy(string, s1, len);
	string[len] = '\0';
	return (string);
}

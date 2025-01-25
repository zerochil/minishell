/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:45:05 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_length;

	if (*needle == '\0')
		return ((char *)haystack);
	needle_length = ft_strlen(needle);
	while (*haystack && len >= needle_length)
	{
		if (*haystack == *needle && !ft_strncmp(haystack, needle,
				needle_length))
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	needle_length;

	if (*needle == '\0')
		return ((char *)haystack);
	needle_length = ft_strlen(needle);
	while (*haystack)
	{
		if (*haystack == *needle && !ft_strncmp(haystack, needle,
				needle_length))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}

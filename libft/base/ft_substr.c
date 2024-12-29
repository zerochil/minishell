/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 08:55:34 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:45 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_length;
	char	*string;

	if (!s)
		return (NULL);
	s_length = ft_strlen(s);
	if (s_length <= start)
		return (ft_strdup(""));
	if (len > s_length - start)
		len = s_length - start;
	string = malloc(len + 1);
	if (!string)
		return (NULL);
	ft_strlcpy(string, s + start, len + 1);
	return (string);
}

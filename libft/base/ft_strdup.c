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

	len = ft_strlen(s1) + 1;
	string = malloc(len);
	if (!string)
		return (NULL);
	ft_memcpy(string, s1, len);
	return (string);
}

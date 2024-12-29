/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 01:47:20 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 17:03:07 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strchr(const char *str, int c)
{
	char	character;

	if (!str)
		return (NULL);
	character = c;
	while (*str && *str != character)
		str++;
	if (*str == character)
		return ((char *)str);
	return (NULL);
}

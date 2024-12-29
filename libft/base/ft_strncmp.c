/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:04:49 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n-- > 0 && *s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (n == (size_t)-1)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

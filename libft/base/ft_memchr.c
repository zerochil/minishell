/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:04:07 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 15:30:45 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	uc;

	ptr = (unsigned char *)s;
	uc = c;
	while (n--)
	{
		if (*ptr == uc)
			return (ptr);
		ptr++;
	}
	return (NULL);
}

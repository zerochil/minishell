/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:11:39 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 18:15:04 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dst;
	s = src;
	while (len-- > 0)
		*d++ = *s++;
	return (dst);
}

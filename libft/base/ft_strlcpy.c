/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 03:42:15 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/21 20:45:17 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_length;
	size_t	copy_length;

	src_length = ft_strlen(src);
	if (dstsize == 0)
		return (src_length);
	if (src_length < dstsize - 1)
		copy_length = src_length;
	else
		copy_length = dstsize - 1;
	ft_memcpy(dst, src, copy_length);
	dst[copy_length] = '\0';
	return (src_length);
}

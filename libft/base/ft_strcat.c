/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:40:52 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 10:12:45 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strcat(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	while (*dest)
		dest++;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}

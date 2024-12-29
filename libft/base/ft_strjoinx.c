/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:04:49 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strjoinx(char **src)
{
	size_t	len;
	char	*dst;
	char	*ret;
	char	**temp;

	if (!src)
		return (NULL);
	len = 0;
	temp = src;
	while (*temp)
	{
		len += ft_strlen(*temp);
		temp++;
	}
	dst = ft_calloc(len + 1, sizeof(char));
	if (!dst)
		return (NULL);
	ret = dst;
	temp = src;
	while (*temp)
	{
		ft_strcat(dst, *temp);
		temp++;
	}
	return (ret);
}

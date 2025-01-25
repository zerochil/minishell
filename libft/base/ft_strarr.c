/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:34:45 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:34:51 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

size_t	ft_strarr_len(char **arr)
{
	size_t	len;

	len = 0;
	while (arr[len])
		len++;
	return (len);
}

void	ft_strarr_del(char **arr, int index)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (i != index)
			i++;
		else
		{
			ft_memmove(arr + i, arr + i + 1, sizeof(char *)
				* (ft_strarr_len(arr) - i));
			break ;
		}
	}
}

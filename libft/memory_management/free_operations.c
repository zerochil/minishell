/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:34:03 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:38:02 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	free_strings(void *ptr)
{
	char	**temp;
	char	**strings;

	if (ptr == NULL)
		return ;
	strings = ptr;
	temp = strings;
	while (*temp)
	{
		free(*temp);
		temp++;
	}
	free(strings);
}

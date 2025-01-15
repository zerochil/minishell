/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:59:58 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	*array_find(t_array *array, void *target, int (*cmp)(void *, void *))
{
	size_t	i;

	if (!array || !cmp)
	{
		ft_putendl_fd("Error: array_find_element", 2);
		return (NULL);
	}
	i = 0;
	while (i < array->size)
	{
		if (cmp(array->data[i], target))
			return (array->data[i]);
		i++;
	}
	return (NULL);
}

int	array_index_of(t_array *array, void *target, int (*cmp)(void *, void *))
{
	size_t	i;

	if (!array || !cmp)
	{
		ft_putendl_fd("Error: array_find", 2);
		return (-1);
	}
	i = 0;
	while (i < array->size)
	{
		if (cmp(array->data[i], target))
			return (i);
		i++;
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_iterator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 15:34:42 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	*array_next(t_array *array)
{
	void	*element;

	if (array == NULL)
	{
		ft_putendl_fd("array_next: array is NULL", STDERR_FILENO);
		return (NULL);
	}
	if (array->iterator >= array->size)
		return (NULL);
	element = array->data[array->iterator];
	array->iterator++;
	return (element);
}

void	*array_prev(t_array *array)
{
	if (array == NULL)
	{
		ft_putendl_fd("array_prev: array is NULL", STDERR_FILENO);
		return (NULL);
	}
	if (array->iterator == 0)
		return (NULL);
	array->iterator--;
	return (array->data[array->iterator]);
}

void	*array_reset(t_array *array)
{
	if (array == NULL)
	{
		ft_putendl_fd("array_reset: array is NULL", STDERR_FILENO);
		return (NULL);
	}
	array->iterator = 0;
	return (array->data[array->iterator]);
}

void	array_next_till(t_array *array, bool (*is_breakpoint)(void *))
{
	if (array == NULL || is_breakpoint == NULL)
	{
		ft_putendl_fd("array_next_till: an param is NULL", STDERR_FILENO);
		return ;
	}
	if (array->iterator >= array->size)
		return ;
	while (array->iterator < array->size)
	{
		if (is_breakpoint(array_next(array)))
			break ;
	}
}

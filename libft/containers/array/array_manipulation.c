/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_manipulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/22 12:48:18 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	array_push(t_array *array, void *item)
{
	if (!array)
	{
		ft_putendl_fd("Error: array_push", 2);
		return ;
	}
	array_ensure_capacity(array, array->size + 1);
	array->data[array->size++] = item;
}

void	*array_pop(t_array *array)
{
	void	*item;

	if (!array || array->size == 0)
	{
		ft_putendl_fd("Error: array_pop", 2);
		return (NULL);
	}
	item = array->data[array->size - 1];
	array->data[array->size - 1] = NULL;
	array->size--;
	return (item);
}

void	*array_remove(t_array *array, size_t index)
{
	void	*item;

	if (!array)
		return (ft_putendl_fd("Error: array_remove: array is NULL",
				STDERR_FILENO), NULL);
	if (array->size == 0)
		return (NULL);
	if (index >= array->size)
		return (ft_putendl_fd("Error: array_remove: index too great",
				STDERR_FILENO), NULL);
	item = array->data[index];
	ft_memmove(array->data + index, array->data + index + 1, (array->size
			- index - 1) * sizeof(void *));
	array->data[array->size - 1] = NULL;
	array->size--;
	return (item);
}

void	*array_shift(t_array *array)
{
	return (array_remove(array, 0));
}

void	array_insert(t_array *array, size_t index, void *element)
{
	if (!array || index > array->size)
	{
		ft_putendl_fd("Error: array_insert", 2);
		return ;
	}
	array_ensure_capacity(array, array->size + 1);
	ft_memmove(array->data + index + 1, array->data + index, (array->size
			- index) * sizeof(void *));
	array->data[index] = element;
	array->size++;
}

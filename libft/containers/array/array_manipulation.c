/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_manipulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/19 10:57:59 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	array_push(t_array *array, void *item)
{
	void	**new_data;
	size_t	new_cap;

	if (!array)
		return ;
	if (array->size == array->capacity)
	{
		new_cap = array->capacity * 2;
		new_data = container_grow(array->data, array->capacity, new_cap,
				sizeof(void *));
		array->data = new_data;
		array->capacity = new_cap;
	}
	array->data[array->size++] = item;
}

void	*array_pop(t_array *array)
{
	void	*item;

	if (!array || array->size == 0)
		return (NULL);
	item = array->data[array->size - 1];
	array->data[array->size - 1] = NULL;
	array->size--;
	return (item);
}

void	array_remove(t_array *array, size_t index)
{
	if (!array || index >= array->size)
		return ;
	while (index < array->size - 1)
	{
		array->data[index] = array->data[index + 1];
		index++;
	}
	array->data[array->size - 1] = NULL;
	array->size--;
}

void	array_insert(t_array *array, size_t index, void *element)
{
	size_t	i;

	if (!array || index > array->size)
		return ;
	if (array->size >= array->capacity)
		array->data = container_grow(array->data, array->capacity, array->capacity * 2,
				sizeof(void *));
	i = array->size;
	while (i > index)
	{
		array->data[i] = array->data[i - 1];
		i--;
	}
	array->data[index] = element;
	array->size++;
}

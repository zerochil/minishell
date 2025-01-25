/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/10 16:56:59 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	array_init(t_array *array)
{
	if (!array)
		return ;
	array->data = track_malloc(INITIAL_CAPACITY * sizeof(void *));
	array->size = 0;
	array->capacity = INITIAL_CAPACITY;
}

void	array_destroy(t_array *array)
{
	if (!array)
		return ;
	resource_free(array->data);
	array->data = NULL;
	array->size = 0;
	array->capacity = 0;
}

t_array	*array_create(void)
{
	t_array	*array;

	array = track_malloc(sizeof(t_array));
	array_init(array);
	return (array);
}

void	array_ensure_capacity(t_array *array, size_t needed_capacity)
{
	size_t	new_cap;

	if (needed_capacity <= array->capacity)
		return ;
	new_cap = array->capacity;
	while (new_cap < needed_capacity)
		new_cap *= 2;
	array->data = container_grow(array->data, array->capacity, new_cap,
			sizeof(void *));
	array->capacity = new_cap;
}

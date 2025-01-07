/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/07 21:17:22 by inajah           ###   ########.fr       */
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

void	array_ensure_capacity(t_array *array, size_t needed_capacity)
{
	size_t	new_cap;

	needed_capacity = needed_capacity + 1;
	if (needed_capacity >= array->capacity)
	{
		new_cap = array->capacity * 2;
		array->data = container_grow(array->data, array->capacity, new_cap,
				sizeof(void *));
		array->capacity = new_cap;
	}
}

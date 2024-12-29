/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/19 10:57:59 by rrochd           ###   ########.fr       */
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
	free_resource(array->data);
	array->data = NULL;
	array->size = 0;
	array->capacity = 0;
}

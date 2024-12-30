/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_helper_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 15:00:02 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	array_do(t_array *array, void (*func)(void *))
{
	size_t	i;

	if (!array || !func)
		return ;
	i = 0;
	while (i < array->size)
	{
		func(array->data[i]);
		i++;
	}
}

void	*array_dup(t_array *array)
{
	void	**new_data;

	if (!array || array->size == 0)
		return (NULL);
	new_data = track_malloc((array->size + 1) * sizeof(void *));
	ft_memcpy(new_data, array->data, array->size * sizeof(void *));
	return (new_data);
}

void	free_array(t_array *arr)
{
	array_do(arr, resource_free);
	array_destroy(arr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 17:58:49 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	*array_peek(t_array *array)
{
	if (!array || array->size == 0)
		return (NULL);
	return (array->data[0]);
}

void	*array_peek_last(t_array *array)
{
	if (!array || array->size == 0)
		return (NULL);
	return (array->data[array->size - 1]);
}

void *array_get(t_array *array, size_t index)
{
	if (array->size == 0)
		return (NULL);
	if (!array || index >= array->size)
	{
		ft_putendl_fd("Error: array_get", 2);
		return (NULL);
	}
	return (array->data[index]);
}

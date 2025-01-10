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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_bulk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:36:22 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:36:23 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	*array_replace(t_array *array, size_t index, void *element)
{
	void	*old_element;

	if (!array || index >= array->size)
	{
		ft_putendl_fd("Error: array_replace", 2);
		return (NULL);
	}
	old_element = array->data[index];
	array->data[index] = element;
	return (old_element);
}

void	array_merge(t_array *dest, t_array *src)
{
	size_t	i;

	if (!dest || !src)
	{
		ft_putendl_fd("Error: array_merge", 2);
		return ;
	}
	i = 0;
	while (i < src->size)
	{
		array_push(dest, src->data[i]);
		i++;
	}
}

void	array_expand_at(t_array *array, size_t index, t_array *subarray)
{
	ssize_t	i;

	if (!array || !subarray)
	{
		report_error("array_expand_at: error");
		return ;
	}
	array_remove(array, index);
	i = subarray->size - 1;
	while (i >= 0)
	{
		array_insert(array, index, subarray->data[i]);
		i--;
	}
}

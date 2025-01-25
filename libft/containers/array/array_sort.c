/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 18:30:04 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 18:30:04 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void	array_sort(t_array *array, int (*cmp)(void *, void *))
{
	size_t	i;
	size_t	j;
	void	*temp;

	if (!array || !cmp || array->size < 2)
		return ;
	i = 0;
	while (i < array->size - 1)
	{
		j = 0;
		while (j < array->size - i - 1)
		{
			if (cmp(array->data[j], array->data[j + 1]) > 0)
			{
				temp = array->data[j];
				array->data[j] = array->data[j + 1];
				array->data[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

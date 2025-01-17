/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:00:43 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/17 09:56:25 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
# define ARRAY_H

# include "../_containers_dependencies.h"
# include "../../base/base.h"

typedef struct s_array
{
	void	**data;
	size_t	size;
	size_t	capacity;
	size_t	iterator;
}			t_array;

void		array_init(t_array *arr);
void		array_destroy(t_array *arr);
void	array_ensure_capacity(t_array *array, size_t needed_capacity);


void	*array_next(t_array *array);
void	*array_reset(t_array *array);
void		array_push(t_array *arr, void *item);
void		array_insert(t_array *array, size_t index, void *element);
void		*array_pop(t_array *array);
void		*array_remove(t_array *arr, size_t index);
void		*array_shift(t_array *arr);
void *array_replace(t_array *array, size_t index, void *element);
void	array_merge(t_array *dest, t_array *src);

void		array_do(t_array *array, void (*func)(void *));
void		*array_dup(t_array *arr);
void		free_array(t_array *array);

void	*array_peek(t_array *array);
void	*array_peek_last(t_array *array);
void *array_get(t_array *array, size_t index);
void	*array_find(t_array *array, void *target, int (*cmp)(void *, void *));
int		array_index_of(t_array *array, void *target, int (*cmp)(void *, void *));

void    array_sort(t_array *array, int (*cmp)(void *, void *));

#endif

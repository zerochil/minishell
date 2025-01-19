/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:20:04 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/19 18:53:07 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	*safe_malloc(size_t size)
{
	void	*allocated_space;

	allocated_space = ft_calloc(size, 1);
	if (allocated_space == NULL)
		error("Safe Callocation failed; BUY MORE RAM!!!");
	return (allocated_space);
}

void	*track_malloc(size_t size)
{
	void	*ptr;

	ptr = safe_malloc(size);
	resource_track(ptr, free);
	return (ptr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:36:01 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/11 21:32:16 by rrochd           ###   ########.fr       */
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
	void	*allocated_space;

	allocated_space = safe_malloc(size);
	track_resource(allocated_space, free);
	return (allocated_space);
}

int	track_open(char *pathname, int flags, mode_t mode, char *error_message)
{
	int	fd;

	fd = open(pathname, flags, mode);
	if (fd < 0)
		error(error_message);
	track_resource((void *)(intptr_t)fd, close_fd);
	return (fd);
}

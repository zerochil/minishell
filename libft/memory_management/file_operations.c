/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:49:03 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:49:04 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

static void	track_close_fd(void *fd_ptr)
{
	int	fd;

	fd = (int)(intptr_t)fd_ptr;
	if (fd >= 0)
		close(fd);
}

int	track_open(char *pathname, int flags, mode_t mode, char *error_message)
{
	int	fd;

	fd = open(pathname, flags, mode);
	if (fd < 0)
		report_error(error_message);
	resource_track((void *)(intptr_t)fd, track_close_fd);
	return (fd);
}

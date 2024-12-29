/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:34:03 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/10 18:34:04 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	free_resource(void *resource)
{
	t_manager	**manager;
	t_resource	*prev;
	t_resource	*current;

	manager = get_resource_managers();
	if (manager == NULL || *manager == NULL)
		return ;
	current = (*manager)->resources;
	prev = NULL;
	while (current)
	{
		if (current->resource == resource)
		{
			if (prev)
				prev->next = current->next;
			else
				(*manager)->resources = current->next;
			current->cleanup(resource);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_all_resources(void)
{
	t_manager	**manager;

	manager = get_resource_managers();
	while (*manager)
		end_scope();
}

void	free_strings(void *ptr)
{
	char	**temp;
	char	**strings;

	if (ptr == NULL)
		return ;
	strings = ptr;
	temp = strings;
	while (*temp)
	{
		free(*temp);
		temp++;
	}
	free(strings);
}

void	close_fd(void *fd_ptr)
{
	int	fd;

	fd = (int)(intptr_t)fd_ptr;
	if (fd >= 0)
		close(fd);
}

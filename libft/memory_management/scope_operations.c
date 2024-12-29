/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scope_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:36:16 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/10 18:36:16 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	add_scope(void)
{
	t_manager	**manager;
	t_manager	*new_manager;

	manager = ensure_default_manager();
	new_manager = malloc(sizeof(t_manager));
	if (new_manager == NULL)
		error("Failed to allocate resource manager");
	new_manager->resources = NULL;
	new_manager->next = *manager;
	*manager = new_manager;
}

void	end_scope(void)
{
	t_manager	**manager;
	t_manager	*current_manager;
	t_resource	*current_resource;
	t_resource	*temp;

	manager = get_resource_managers();
	if (*manager == NULL)
		return ;
	current_manager = *manager;
	current_resource = current_manager->resources;
	while (current_resource)
	{
		temp = current_resource;
		temp->cleanup((void *)temp->resource);
		current_resource = current_resource->next;
		free(temp);
	}
	*manager = current_manager->next;
	free(current_manager);
}

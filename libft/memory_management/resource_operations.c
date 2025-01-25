/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:49:13 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:49:13 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

// TODO: GUARD: what if we track the sam resource multiple times??
// this can lead to multiple frees OR we when we call free_resoure
// it should free all the instances of that

void	resource_track(void *resource, void (*cleanup)(void *))
{
	t_rm_node	**manager_node;
	t_rm_node	*new_resource;

	if (cleanup == NULL)
		error("Failed to track resource, no cleanup function");
	manager_node = managers_get_instance();
	new_resource = malloc(sizeof(t_rm_node));
	if (new_resource == NULL)
	{
		cleanup(resource);
		error("Failed to allocate space for resource tracker");
	}
	new_resource->resource.pointer = resource;
	new_resource->resource.cleanup = cleanup;
	rm_node_add_front(&(*manager_node)->manager.resources, new_resource);
}

// TODO: this could make use of the helper functions already defined
void	resource_free(void *resource)
{
	t_rm_node	**manager_node;
	t_rm_node	**indirect;
	t_rm_node	*to_remove;

	manager_node = managers_get_instance();
	if (manager_node == NULL || *manager_node == NULL)
		return ;
	indirect = &(*manager_node)->manager.resources;
	while (*indirect)
	{
		if ((*indirect)->resource.pointer == resource)
		{
			to_remove = *indirect;
			*indirect = (*indirect)->next;
			to_remove->resource.cleanup(to_remove->resource.pointer);
			free(to_remove);
			return ;
		}
		indirect = &(*indirect)->next;
	}
}

void	resource_transfer(void *resource_pointer, char *target_manager_name)
{
	t_rm_node	**managers_list;
	t_rm_node	*manager_node;
	t_rm_node	*resource_node;
	t_rm_node	*target_manager_node;

	managers_list = managers_get_instance();
	if (*managers_list == NULL)
		return ;
	manager_node = *managers_list;
	target_manager_node = rm_node_find(managers_list, target_manager_name,
			match_manager);
	if (target_manager_node == NULL)
		return ;
	resource_node = rm_node_unlink(&manager_node->manager.resources,
			resource_pointer, match_resource);
	if (resource_node == NULL)
		return ;
	rm_node_add_front(&target_manager_node->manager.resources, resource_node);
}

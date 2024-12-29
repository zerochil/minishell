/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_resources.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:34:07 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/10 18:34:08 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	track_resource(void *resource, void (*cleanup)(void *))
{
	t_manager	**manager;
	t_resource	*new_resource;

	if (cleanup == NULL)
		error("Failed to track resource, no cleanup function");
	manager = ensure_default_manager();
	new_resource = malloc(sizeof(t_resource));
	if (new_resource == NULL)
	{
		cleanup(resource);
		error("Failed to allocate space for resource tracker");
	}
	new_resource->resource = resource;
	new_resource->cleanup = cleanup;
	new_resource->next = (*manager)->resources;
	(*manager)->resources = new_resource;
}

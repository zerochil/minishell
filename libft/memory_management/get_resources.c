/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_resources.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:36:30 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/10 18:36:31 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

t_manager	**get_resource_managers(void)
{
	static t_manager	*manager = NULL;

	return (&manager);
}

t_manager	**ensure_default_manager(void)
{
	t_manager	**manager;

	manager = get_resource_managers();
	if (*manager == NULL)
	{
		*manager = malloc(sizeof(t_manager));
		if (*manager == NULL)
			error("Failed to allocate default resource manager");
		(*manager)->resources = NULL;
		(*manager)->next = NULL;
	}
	return (manager);
}

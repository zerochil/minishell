/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_operations_predicates.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:17:28 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:17:29 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

int	match_resource(void *node_ptr, void *target_resource_pointer)
{
	t_rm_node	*node;

	node = node_ptr;
	return (node->resource.pointer == target_resource_pointer);
}

int	match_manager(void *node_ptr, void *target_manger_name_ptr)
{
	t_rm_node	*node;
	char		*target_manger_name;

	target_manger_name = target_manger_name_ptr;
	node = node_ptr;
	return (ft_strcmp(node->manager.name, target_manger_name) == 0);
}

void	rm_node_del_resource(void *node_ptr)
{
	t_rm_node	*node;

	node = node_ptr;
	node->resource.cleanup(node->resource.pointer);
	free(node);
}

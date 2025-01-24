/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 01:07:53 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 15:02:48 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

t_rm_node	**managers_get_instance(void)
{
	static t_rm_node	*head;

	if (head == NULL)
		head = rm_node_new_manager("default");
	return (&head);
}

void	manager_add(char *name)
{
	t_rm_node	**managers_list;

	managers_list = managers_get_instance();
	rm_node_add_front(managers_list, rm_node_new_manager(name));
}

void	manager_free(void)
{
	t_rm_node	**managers_list;
	t_rm_node	*manager_node;

	managers_list = managers_get_instance();
	if (*managers_list == NULL)
		return ;
	manager_node = *managers_list;
	rm_node_clear_list(&manager_node->manager.resources, rm_node_del_resource);
	*managers_list = manager_node->next;
	free(manager_node);
}

void	manager_free_everything(void)
{
	t_rm_node	**manager;
	t_scope		*scope;

	manager = managers_get_instance();
	scope = get_scope();
	while (scope->stack)
		manager_scope_end();
	while (*manager)
		manager_free();
}

t_rm_node	*rm_node_new_manager(char *name)
{
	t_rm_node	*node;

	node = safe_malloc(sizeof(t_rm_node));
	node->manager.name = name;
	node->next = NULL;
	return (node);
}

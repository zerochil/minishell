/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_scope_operations.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 01:07:53 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 15:02:48 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

t_scope	*get_scope(void)
{
	static t_scope	scope;

	return (&scope);
}

void	manager_scope_begin(char *name)
{
	t_scope			*scope;
	t_rm_node		**current;
	t_rm_node		*target_manager;
	t_scope_node	*new_scope_node;

	current = managers_get_instance();
	scope = get_scope();
	if (scope->stack == NULL)
		scope->original_head = *current;
	target_manager = rm_node_find(current, name, match_manager);
	if (target_manager == NULL)
	{
		target_manager = rm_node_new_manager(name);
		rm_node_add_back(&scope->original_head, target_manager);
	}
	new_scope_node = safe_malloc(sizeof(t_scope_node));
	new_scope_node->next = scope->stack;
	scope->stack = new_scope_node;
	scope->stack->head = target_manager;
	*current = target_manager;
}

void	manager_scope_end(void)
{
	t_scope			*scope;
	t_scope_node	*old_state;
	t_rm_node		**current;

	scope = get_scope();
	if (scope->stack == NULL)
		return ;
	current = managers_get_instance();
	old_state = scope->stack;
	scope->stack = scope->stack->next;
	if (scope->stack)
		*current = scope->stack->head;
	else
		*current = scope->original_head;
	free(old_state);
}

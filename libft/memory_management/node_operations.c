/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:19:09 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:25:53 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	rm_node_add_front(t_rm_node **lst, t_rm_node *new_node)
{
	if (lst == NULL || new_node == NULL)
		return ;
	new_node->next = *lst;
	*lst = new_node;
}

t_rm_node	*rm_node_add_back(t_rm_node **lst, t_rm_node *new_node)
{
	t_rm_node	*current;

	if (!*lst)
		return (*lst = new_node);
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (new_node);
}

void	rm_node_clear_list(t_rm_node **lst, void (*del)(void *))
{
	t_rm_node	*next;
	t_rm_node	*tmp;

	if (lst == NULL || del == NULL)
		return ;
	next = *lst;
	while (next)
	{
		tmp = next->next;
		del(next);
		next = tmp;
	}
	*lst = NULL;
}

t_rm_node	*rm_node_unlink(t_rm_node **head, void *target, int (*match)(void *,
			void *))
{
	t_rm_node	**indirect;
	t_rm_node	*node_to_unlink;

	indirect = head;
	if (!head || !match)
		return (NULL);
	while (*indirect)
	{
		if (match(*indirect, target))
		{
			node_to_unlink = *indirect;
			*indirect = node_to_unlink->next;
			node_to_unlink->next = NULL;
			return (node_to_unlink);
		}
		indirect = &(*indirect)->next;
	}
	return (NULL);
}

t_rm_node	*rm_node_find(t_rm_node **head, void *target, int (*match)(void *,
			void *))
{
	t_rm_node	**indirect;

	indirect = head;
	if (!head || !match)
		return (NULL);
	while (*indirect)
	{
		if (match(*indirect, target))
			return (*indirect);
		indirect = &(*indirect)->next;
	}
	return (NULL);
}

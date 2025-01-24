/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_predicates.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:20:39 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 15:24:05 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

bool	is_binary_or(void *node_ptr)
{
	t_ast_node	*node;

	node = (t_ast_node *)node_ptr;
	return (node->type == AST_BINARY_OR);
}

bool	is_binary_and(void *node_ptr)
{
	t_ast_node	*node;

	node = (t_ast_node *)node_ptr;
	return (node->type == AST_BINARY_AND);
}

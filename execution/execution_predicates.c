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

bool	is_same_tty(char *filename)
{
	struct stat	tty_stat;
	struct stat	file_stat;

	if (fstat(STDIN_FILENO, &tty_stat) == -1)
		return (report_error("minishell: fstat failed"), false);
	if (stat(filename, &file_stat) == -1)
		return (false);
	return (file_stat.st_dev == tty_stat.st_dev
		&& file_stat.st_ino == tty_stat.st_ino);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 15:37:48 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int	execution(t_array *ast_root_list)
{
	t_ast_node	*ast_root;
	int			status;

	while (true)
	{
		ast_root = array_shift(ast_root_list);
		if (ast_root == NULL)
			break ;
		if (ast_root->type == AST_INVALID_COMMAND)
		{
			report_error(ast_root->error_message);
			status = 2;
			continue ;
		}
		status = execute_compound_command(ast_root);
	}
	return (status);
}

int	execute_compound_command(t_ast_node *node)
{
	int			last_exit_status;
	t_ast_node	*command_node;

	last_exit_status = 0;
	while (true)
	{
		command_node = array_shift(node->children);
		if (command_node == NULL)
			break ;
		if (command_node->type == AST_PIPELINE)
		{
			last_exit_status = execute_pipeline(command_node);
			ctx_exit_status(CTX_SET, last_exit_status);
			if (last_exit_status == EXIT_STATUS_SIGINT)
				return (last_exit_status);
			continue ;
		}
		if ((command_node->type == AST_BINARY_AND && last_exit_status != 0)
			|| (command_node->type == AST_BINARY_OR && last_exit_status == 0))
			break ;
		if (command_node->type != AST_BINARY_AND
			&& command_node->type != AST_BINARY_OR)
			error("execute_compound_command: error");
	}
	return (last_exit_status);
}

int	execute_command(t_ast_node *node)
{
	handle_expansions(node);
	if (node->type == AST_SUBSHELL)
		return (execute_subshell(node));
	else if (node->type == AST_SIMPLE_COMMAND)
		return (execute_simple_command(node));
	else
		error("execute_command_list: error");
	return (-1);
}

int	execute_subshell(t_ast_node *node)
{
	int			status;
	size_t		index;
	t_stream	stream;

	stream = (t_stream){.read = STDIN_FILENO, .write = STDOUT_FILENO};
	if (open_redirection_files(node->redirect_list, &stream) == -1)
		return (-1);
	stream_dup2stdio(&stream);
	index = 0;
	while (index < node->children->size)
	{
		status = execute_compound_command(array_get(node->children, index));
		if (status == EXIT_STATUS_SIGINT)
			break ;
		index++;
	}
	destroy_context();
	exit(status);
}

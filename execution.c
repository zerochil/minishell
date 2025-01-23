/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 15:35:13 by rrochd           ###   ########.fr       */
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
	int			exit_status;
	t_ast_node	*command_node;

	exit_status = 0;
	array_reset(node->children);
	while (true)
	{
		command_node = array_next(node->children);
		if (command_node == NULL)
			break ;
		if (command_node->type == AST_PIPELINE)
		{
			exit_status = execute_pipeline(command_node);
			ctx_exit_status(CTX_SET, exit_status);
			if (exit_status == EXIT_STATUS_SIGINT)
				return (EXIT_STATUS_SIGINT);
		}
		else if ((command_node->type == AST_BINARY_OR && exit_status == 0))
			array_next_till(node->children, is_binary_and);
		else if ((command_node->type == AST_BINARY_AND && exit_status != 0))
			array_next_till(node->children, is_binary_or);
	}
	return (exit_status);
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

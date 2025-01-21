/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_simple_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:18:23 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtins.h"

int interpret_execve_error(void)
{
	if (errno == ENOENT)
		return (127);
	return (126);
}

int	execute_external(t_command_context *command_context)
{
	char	*command_path;
//TODO: check if command path is a directory, return 126 and error message: Is a directory
	stream_dup2stdio(&command_context->stream);
	command_path = get_command_path(command_context->args[0]);
	if (command_path == NULL)
	{
		display_error(SHELL_NAME, command_context->args[0], ERR_COMMAND_NOT_FOUND);
		return (127);
	}
	if (is_directory(command_path))
	{
		display_error(SHELL_NAME, command_context->args[0], ERR_IS_DIRECTORY);
		return (126);
	}
	command_context->args[0] = command_path;
	execve(command_context->args[0], command_context->args,
		command_context->envp);
	display_error(SHELL_NAME, command_context->args[0], strerror(errno));
	//perror("minishell: external");
	return (interpret_execve_error()); // TODO: check if this is correct
}

int	execute_builtin(char **args, int out_fd)
{
	builtin_t	*builtins;
	int			i;

	builtins = get_builtins_instance();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, args[0]) == 0)
			return (builtins[i].function(args, out_fd));
		i++;
	}
	return (-1);
}

int	execute_simple_command(t_ast_node *node)
{
	t_command_context	command_context;
	int					status;

	command_context.stream = (t_stream){.read = STDIN_FILENO,
		.write = STDOUT_FILENO};
	if (open_redirection_files(node->redirect_list, &command_context.stream) == -1)
		return (1);
	command_context.args = get_arg_list(node->children);
	command_context.envp = env_get_array();
	status = 0;
	if (command_context.args[0] != NULL)
	{
		if (is_builtin(command_context.args[0]))
			status = execute_builtin(command_context.args,
					command_context.stream.write);
		else
			status = execute_external(&command_context);
	}
	stream_close(&command_context.stream);
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_simple_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/24 16:38:40 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>
#include <execution.h>

int	interpret_execve_error(void)
{
	if (errno == ENOENT)
		return (127);
	return (126);
}

int	execute_external(char *command_name, t_command_context *command_context)
{
	stream_dup2stdio(&command_context->stream);
	if (command_context->args[0] == NULL)
	{
		display_error(SHELL_NAME, command_name, ERR_COMMAND_NOT_FOUND);
		return (127);
	}
	if (is_directory(command_context->args[0]))
	{
		display_error(SHELL_NAME, command_context->args[0], ERR_IS_DIRECTORY);
		return (126);
	}
	execve(command_context->args[0], command_context->args,
		command_context->envp);
	display_error(SHELL_NAME, command_context->args[0], strerror(errno));
	return (interpret_execve_error());
}

int	execute_builtin(char *command_name, char **args, int out_fd)
{
	builtin_t	*builtins;
	int			i;

	args[0] = command_name;
	builtins = get_builtins_instance();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, command_name) == 0)
			return (builtins[i].function(args, out_fd));
		i++;
	}
	return (-1);
}

int	execute_simple_command(t_ast_node *node)
{
	t_command_context	cmd_ctx;
	int					status;
	char				*command_name;

	cmd_ctx.stream = (t_stream){.read = STDIN_FILENO,
		.write = STDOUT_FILENO};
	if (open_redirection_files(node->redirect_list, &cmd_ctx.stream) == -1)
		return (1);
	cmd_ctx.args = get_arg_list(node->children);
	command_name = cmd_ctx.args[0];
	cmd_ctx.args[0] = get_command_path(cmd_ctx.args[0]);
	status = 0;
	if (command_name != NULL)
	{
		if (is_builtin(command_name))
			status = execute_builtin(command_name, cmd_ctx.args,
					cmd_ctx.stream.write);
		else
		{
			env_set_key_value("_", cmd_ctx.args[0]);
			cmd_ctx.envp = env_get_array(cmd_ctx.args[0]);
			status = execute_external(command_name, &cmd_ctx);
		}
	}
	return (stream_close(&cmd_ctx.stream), status);
}

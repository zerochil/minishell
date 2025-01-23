/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:19:26 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtins.h"

int	should_not_fork(t_array *commands)
{
	t_ast_node	*command_node;
	t_token		*token;
	t_field		*field;
	char		*command_name;

	if (commands->size != 1)
		return (0);
	command_node = array_get(commands, 0);
	if (command_node->type != AST_SIMPLE_COMMAND)
		return (0);
	token = array_get(command_node->children, 0);
	if (token == NULL)
		return (0);
	field = array_get(token->fields, 0);
	command_name = field->value->data;
	if (is_builtin(command_name) == 0)
		return (0);
	return (1);
}

#include <sys/ioctl.h>
pid_t	fork_and_execute(t_array *commands, t_stream *streamline, size_t index)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error(strerror(errno));
	else if (pid > 0)
		return (pid);
	ctx_is_child(CTX_SET, true);
	setup_child_signals();
	stream_dup2stdio(&streamline[index]);
	if (index != commands->size - 1)
		stream_close(&streamline[index + 1]);
	status = execute_command(array_get(commands, index));
	clean_exit(status);
	return (-1);
}

int	execute_pipeline(t_ast_node *node)
{
	t_pipeline_state	state;
	int					exit_status;

	pipeline_init(&state, node->children);
	// TODO: get this out of here
	if (should_not_fork(state.commands))
		return (execute_command(array_get(state.commands, 0)));
	while (state.index < state.commands->size)
	{
		if (state.index < state.commands->size - 1)
			create_pipe(state.streamline, state.index);
		state.last_pid = fork_and_execute(state.commands, state.streamline,
				state.index);
		stream_close(&state.streamline[state.index]);
		state.index++;
	}
	exit_status = wait_for_pipeline(&state);
	return (exit_status);
}

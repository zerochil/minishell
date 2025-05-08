/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipeline_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/26 08:48:26 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution_pipeline.h>

#define DUMP_MSG "Quit (core dumped)"

void	pipeline_init(t_pipeline_state *state, t_array *commands)
{
	array_do(commands, handle_expansions);
	state->commands = commands;
	state->index = 0;
	state->last_pid = -1;
	state->streamline = streamline_init(commands->size);
}

int	wait_for_pipeline(t_pipeline_state *state)
{
	int		status;
	int		last_status;
	pid_t	pid;
	int		sig;

	last_status = 0;
	while (true)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == state->last_pid)
			last_status = status;
	}
	if (pid == -1 && errno == EINTR)
		return (EXIT_STATUS_SIGINT);
	if (WIFEXITED(last_status))
	{
		ctx_is_foreground(CTX_SET, true);
		return (WEXITSTATUS(last_status));
	}
	sig = WTERMSIG(last_status);
	if (sig == SIGQUIT)
		ft_putendl_fd(DUMP_MSG, STDERR_FILENO);
	return (128 + sig);
}

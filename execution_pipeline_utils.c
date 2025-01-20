/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipeline_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 15:37:48 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution_pipeline.h"

void	pipeline_init(t_pipeline_state *state, t_array *commands)
{
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
		return (SIGINT_EXIT);
	if (WIFSIGNALED(last_status))
	{
		sig = WTERMSIG(last_status);
		if (sig == SIGQUIT)
			return (ft_putendl_fd("Quit (core dumped)", STDERR_FILENO),
				SIGQUIT_EXIT);
		else if (sig == SIGINT)
			return (SIGINT_EXIT);
	}
	return (WEXITSTATUS(last_status));
}

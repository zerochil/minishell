/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:16:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/16 17:05:54 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include "libft/libft.h"
#include "context.h"
#include <sys/wait.h>


void handle_sigint(t_array *pids)
{
	int status;

	if (pids->size == 0)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return ;
	}
	// this is redudant, to remove, array_shift & wait are done in the pipeline.
	while(pids->size > 0)
	{
		wait(&status);
		array_shift(pids);
	}
	ft_putstr_fd("\n", 1);
}

void handle_sigquit(t_array *pids)
{
	if (pids->size == 0)
	{
		rl_redisplay();
		return;
	}
	// this is redudant, to remove, array_shift is done in the pipeline.
	while(pids->size > 0)
		array_shift(pids);
	ft_putstr_fd("Quit: 3\n", 1);
}

void handle_signal(int signal)
{
	t_context *context;

	context = get_context_instance();
	if (signal == SIGINT)
		handle_sigint(context->pids);
	else if (signal == SIGQUIT)
		handle_sigquit(context->pids);
}

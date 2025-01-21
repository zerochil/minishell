/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/21 10:03:15 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = handle_signal;
	sigaction(SIGINT, &sa, NULL);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_here_doc_signals(void)
{
	int	signo;

	signo = 1;
	while (signo < NSIG)
	{
		if (signo != SIGKILL || signo != SIGSTOP)
			signal(signo, SIG_IGN);
		signo++;
	}
}

void	handle_here_doc_signal(int signo)
{
	(void)signo;
	destroy_context();
	exit(130);
}

void	handle_signal(int signo)
{
	t_context	*context;

	context = get_context_instance();
	if (signo == SIGINT)
	{
		if (!context->foreground)
			ft_putchar_fd('\n', STDERR_FILENO);
		else
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

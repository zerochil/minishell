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
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
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
	if (signo != SIGINT)
		return ;
	if (ctx_is_foreground(CTX_GET, CTX_NO_VALUE) == false)
		ft_putchar_fd('\n', STDERR_FILENO);
	else
	{
		ctx_exit_status(CTX_SET, EXIT_STATUS_SIGINT);
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

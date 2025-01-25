/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:32:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <context.h>
# include <readline/readline.h>
# include <signal.h>
# include <utils.h>

# define EXIT_STATUS_SIGINT 130
# define EXIT_STATUS_SIGQUIT 131

void	setup_signals(void);
void	setup_here_doc_signals(void);
void	handle_signal(int signo);
void	handle_here_doc_signal(int signo);
void	setup_child_signals(void);

#endif

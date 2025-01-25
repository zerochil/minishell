/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 10:00:33 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H

# include <errno.h>
# include <sys/wait.h>
# include <signals.h>
# include <stream.h>
# include <expansion.h>

typedef struct s_pipeline_state
{
	t_array		*commands;
	t_stream	*streamline;
	size_t		index;
	pid_t		last_pid;
}				t_pipeline_state;

void			pipeline_init(t_pipeline_state *state, t_array *commands);
int				wait_for_pipeline(t_pipeline_state *state);

#endif

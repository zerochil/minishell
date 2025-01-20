/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:20:12 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STREAM_H
# define STREAM_H

typedef struct s_stream
{
	int		read;
	int		write;
}			t_stream;

t_stream	*streamline_init(int pipeline_size);
void		create_pipe(t_stream *streamline, int index);
void		stream_close(t_stream *stream);
void		stream_dup2stdio(t_stream *stream);

#endif

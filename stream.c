/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 10:16:36 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

t_stream	*streamline_init(int pipeline_size)
{
	t_stream	*streamline;
	int			i;

	streamline = track_malloc(sizeof(t_stream) * (pipeline_size + 1));
	i = 0;
	while (i < pipeline_size)
	{
		streamline[i].read = -1;
		streamline[i].write = -1;
		i++;
	}
	streamline[0].read = STDIN_FILENO;
	streamline[pipeline_size - 1].write = STDOUT_FILENO;
	return (streamline);
}

void	create_pipe(t_stream *streamline, int index)
{
	t_stream	p;

	if (pipe((int *)&p) == -1)
	{
		perror("minishell");
		error(NULL);
	}
	streamline[index].write = p.write;
	streamline[index + 1].read = p.read;
}

void	stream_close(t_stream *stream)
{
	if (stream->read >= 0 && stream->read != STDIN_FILENO)
	{
		close(stream->read);
		stream->read = -1;
	}
	if (stream->write >= 0 && stream->write != STDOUT_FILENO)
	{
		close(stream->write);
		stream->write = -1;
	}
}

void	stream_dup2stdio(t_stream *stream)
{
	if (stream->read != STDIN_FILENO)
		if (dup2(stream->read, STDIN_FILENO) == -1)
			error("dup2: error");
	if (stream->write != STDOUT_FILENO)
		if (dup2(stream->write, STDOUT_FILENO) == -1)
			error("dup2: error");
	stream_close(stream);
}

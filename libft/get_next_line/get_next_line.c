/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:21:36 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 15:03:20 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(t_string *stored, char *newline)
{
	char		*line;
	t_string	temp_string;

	if (stored->data == NULL || stored->size == 0)
		return (NULL);
	if (newline)
	{
		line = ft_substr(stored->data, 0, newline - stored->data + 1);
		string_init(&temp_string);
		string_append(&temp_string, newline + 1);
		string_destroy(stored);
		*stored = temp_string;
	}
	else
	{
		line = ft_strdup(stored->data);
		string_destroy(stored);
	}
	resource_track(line, free);
	return (line);
}

static int	read_buffer(int fd, t_string *stored)
{
	char	*buffer;
	int		bytes_read;

	buffer = track_malloc(BUFFER_SIZE + 1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		string_append(stored, buffer);
	}
	resource_free(buffer);
	return (bytes_read);
}

// TODO: rewrite get_next_line using the new string_op_segment functions
char	*get_next_line(int fd)
{
	static t_string	stored[FOPEN_MAX];
	int				bytes_read;
	char			*newline;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX)
		return (NULL);
	if (!stored[fd].data)
		string_init(&stored[fd]);
	while (1)
	{
		newline = ft_strchr(stored[fd].data, '\n');
		if (newline)
			break ;
		bytes_read = read_buffer(fd, &stored[fd]);
		if (bytes_read <= 0)
			break ;
	}
	return (extract_line(&stored[fd], newline));
}

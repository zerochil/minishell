/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/28 09:16:13 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	handle_append(t_token *token, t_stream *stream, t_field *field)
{
	if (token->type == lexem_get_type("REDIRECTION_APPEND"))
		return (open_file(field->value->data, O_WRONLY | O_CREAT | O_APPEND,
				&stream->write));
	error("handle_redirection: error");
	return (-1);
}

static int	handle_truncate(t_token *token, t_stream *stream, t_field *field)
{
	if (token->type == lexem_get_type("REDIRECTION_TRUNC"))
		return (open_file(field->value->data, O_WRONLY | O_CREAT | O_TRUNC,
				&stream->write));
	return (handle_append(token, stream, field));
}

static int	handle_redirect_in(t_token *token, t_stream *stream, t_field *field)
{
	int fd;

	if (is_same_tty(field->value->data))
		return (true);
	if (token->type == lexem_get_type("HERE_DOCUMENT"))
	{
		fd = open_file(field->value->data, O_RDONLY, &stream->read);
		if (fd >= 0)
			unlink(field->value->data);
		return (fd);
	}
	if (token->type == lexem_get_type("REDIRECTION_IN")
		|| token->type == lexem_get_type("HERE_DOCUMENT"))
		return (open_file(field->value->data, O_RDONLY, &stream->read));
	return (handle_truncate(token, stream, field));
}

int	open_redirection_files(t_array *redirection_list, t_stream *stream)
{
	int		open_error;
	t_token	*token;
	t_field	*field;

	while (true)
	{
		token = array_shift(redirection_list);
		if (token == NULL)
			break ;
		if (token->fields->size != 1)
			return (report_error("minishell: ambiguous redirect"), -1);
		field = array_shift(token->fields);
		open_error = handle_redirect_in(token, stream, field);
		if (open_error == -1)
			return (stream_close(stream), -1);
	}
	return (0);
}

int	open_file(char *filename, int flags, int *fd)
{
	if (*fd != -1 && *fd != STDIN_FILENO && *fd != STDOUT_FILENO)
		close(*fd);
	*fd = open(filename, flags, 0644);
	if (*fd == -1)
	{
		display_error(SHELL_NAME, filename, strerror(errno));
		return (-1);
	}
	return (0);
}

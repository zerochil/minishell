/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 09:12:45 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
		if (token->type == lexem_get_type("REDIRECTION_IN")
			|| token->type == lexem_get_type("HERE_DOCUMENT"))
			open_error = open_file(field->value->data, O_RDONLY, &stream->read);
		else if (token->type == lexem_get_type("REDIRECTION_TRUNC"))
			open_error = open_file(field->value->data,
					O_WRONLY | O_CREAT | O_TRUNC, &stream->write);
		else if (token->type == lexem_get_type("REDIRECTION_APPEND"))
			open_error = open_file(field->value->data,
					O_WRONLY | O_CREAT | O_APPEND, &stream->write);
		else
			error("handle_redirection: error");
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

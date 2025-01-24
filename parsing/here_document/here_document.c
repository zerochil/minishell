/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/24 09:25:07 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_document.h"

void	start_here_doc_prompt(t_string *here_doc, char *delimiter)
{
	char	*line;

	if (here_doc->size > 0)
		string_append(here_doc, "\n");
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf(WARN_HEREDOC_DELIM" (wanted `%s')\n", delimiter);
			break ;
		}
		if (is_delimiter(line, delimiter))
			break ;
		string_append(here_doc, line);
		string_append(here_doc, "\n");
		free(line);
	}
	free(line);
	return ;
}

bool	create_here_doc_temp_file(char *filename, t_string *here_doc,
		bool should_expand)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0 || here_doc == NULL || here_doc->data == NULL)
	{
		report_error("Error: create_here_doc_temp_file");
		return (false);
	}
	if (should_expand)
		expand_parameter_here_doc(here_doc);
	write(fd, here_doc->data, here_doc->size);
	close(fd);
	return (true);
}

bool	here_document_child_process(char *filename, t_string *here_doc,
		t_field *delimiter, bool should_expand)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		error(strerror(errno));
	else if (pid == 0)
	{
		setup_here_doc_signals();
		signal(SIGINT, handle_here_doc_signal);
		start_here_doc_prompt(here_doc, delimiter->value->data);
		create_here_doc_temp_file(filename, here_doc, should_expand);
		clean_exit(0);
	}
	wait(&status);
	tcsetattr(0, TCSANOW, ctx_old_termios(CTX_GET));
	if (status != 0)
	{
		ctx_exit_status(CTX_SET, WEXITSTATUS(status));
		return (false);
	}
	return (true);
}

bool	here_document(char *filename, t_string *input, t_field *delimiter)
{
	t_string	*here_doc;
	size_t		end;
	size_t		start;
	bool		should_expand;
	char		*line;

	here_doc = track_malloc(sizeof(t_string));
	string_init(here_doc);
	should_expand = (field_remove_original_quotes(delimiter) == false);
	start = get_second_line(input->data, 0);
	while (start < input->size)
	{
		end = get_second_line(input->data, start);
		line = string_segment_slice(input, start, end - start);
		if (is_delimiter(line, delimiter->value->data))
			return (create_here_doc_temp_file(filename,
					here_doc, should_expand));
		string_append(here_doc, line);
	}
	return (here_document_child_process(filename, here_doc,
			delimiter, should_expand));
}

char	*create_here_document(t_string *input, t_field *delimiter)
{
	char	*filename;

	filename = random_filename();
	if (here_document(filename, input, delimiter))
		return (filename);
	return (NULL);
}

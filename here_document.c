/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 08:42:10 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_document.h"

bool	is_delimiter(char *line, char *delimiter)
{
	size_t	line_length;
	size_t	delimiter_length;

	line_length = ft_strlen(line);
	if (line_length == 1 && line[0] == '\n')
		return (false);
	delimiter_length = ft_strlen(delimiter);
	if (line_length > 0 && line[line_length - 1] == '\n')
		line_length--;
	if (delimiter_length > line_length)
		line_length = delimiter_length;
	return (ft_strncmp(line, delimiter, line_length) == 0);
}

static size_t	get_second_line(char *input, size_t start)
{
	char in_quote;
	char c;
	size_t	i;

	in_quote = '\0';
	i = start;
	while (input[i])
	{
		c = input[i];
		if (c == '\0')
			break;
		if (in_quote == '\0' && (c == '\'' || c == '"'))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (in_quote == '\0' && c == '\n')
			return (i + 1);
		i++;
	}
	return i;
}

void	start_here_doc_prompt(t_string *here_doc, char *delimiter)
{
	char *line;

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

int	expand_parameter(t_string *string, char *parameter_name);
char *get_parameter_name(t_string *string, size_t dolar_position);

void expand_parameter_here_doc(t_string *content)
{
	char 	*parameter_name;
	char	c;

	string_peek_reset(content);
	while (true)
	{
		c = string_peek(content);
		if (c == '\0')
			break ;
		if (c == '$')
		{
			parameter_name = get_parameter_name(content, content->peek);
			if (expand_parameter(content, parameter_name) > 0)
				continue;
		}
		string_peek_advance(content);
	}
}


bool create_here_doc_temp_file(char *filename, t_string *here_doc, bool should_expand)
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



#include <termios.h>

bool here_document_child_process(char *filename, t_string *here_doc, t_field *delimiter, bool should_expand)
{
	pid_t pid;
	int	status;

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
	tcsetattr(0, TCSANOW, get_old_termios());
	if (status != 0)
	{
		set_exit_status(WEXITSTATUS(status));
		return (false);
	}
	return (true);
}

bool here_document(char *filename, t_string *input, t_field *delimiter)
{
	t_string *here_doc;
	size_t	end;
	size_t	start;
	char *line;
	bool should_expand;

	here_doc = track_malloc(sizeof(t_string));
	string_init(here_doc);
	should_expand = (remove_quotes_from_field(delimiter) == false);
	start = get_second_line(input->data, 0);
	while (start < input->size)
	{
		end = get_second_line(input->data, start);
		line = string_segment_slice(input, start, end - start);
		if (is_delimiter(line, delimiter->value->data))
			return (create_here_doc_temp_file(filename, here_doc, should_expand));
		string_append(here_doc, line);
	}
	return (here_document_child_process(filename, here_doc, delimiter, should_expand));
}

static char	*random_filename(void)
{
	char	*path;
	char	*id;
	size_t	size;
	size_t	x;
	size_t	len;

	size = 256;
	path = safe_malloc(size * sizeof(char));
	resource_track(path, free);
	ft_strlcpy(path, "/tmp/.here_doc_", size);
	len = ft_strlen(path);
	x = (size_t) path;
	id = ft_itoa(x);
	ft_strlcat(path, id, size);
	free(id);
	while (access(path, F_OK) == 0)
	{
		x++;
		id = ft_itoa(x);
		path[len] = '\0';
		ft_strlcat(path, id, size);
		free(id);
	}
	return (path);
}

char *create_here_document(t_string *input, t_field *delimiter)
{
	char	*filename;

	filename = random_filename();
	if (here_document(filename, input, delimiter))
		return (filename);
	return (NULL);
}


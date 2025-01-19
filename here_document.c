/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/19 20:52:37 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_document.h"

#define ERR_HEREDOC_DELIM "minishell: warning: here_document delimited by end-of-file"

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

static size_t start_here_document_prompt(t_string *input, char *delimiter)
{
	char *line;

	string_append(input, "\n");
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf(ERR_HEREDOC_DELIM" (wanted `%s')\n", delimiter);
			break ;
		}
		if (is_delimiter(line, delimiter))
			break ;
		string_append(input, line);
		string_append(input, "\n");
		free(line);
	}
	free(line);
	return (input->size);
}

int	expand_parameter(t_string *string, char *parameter_name);
char *get_parameter_name(t_string *string, size_t dolar_position);

static size_t get_delimiter_position(t_string *input, size_t start, char *delimiter)
{
	size_t	end;
	char *tmp;

	while (start < input->size)
	{
		//printf("# input: \"%s\",size: %ld, start: %ld\n", input->data, input->size, start);
		end = get_second_line(input->data, start);
		//printf("# end: %ld\n", end);
		tmp = string_segment_extract(input, start, end - start);
		//printf("slice: %s\n", tmp);
		if (is_delimiter(tmp, delimiter))
			return (start);
		start = end;
	}
	return (start_here_document_prompt(input, delimiter));
}

static void expand_parameter_heredoc(t_string *content)
{
	char 	*parameter_name;
	char	c;

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

char *get_here_document_content(t_string *input, t_field *delimiter)
{
	t_string content;
	bool delimiter_quoted;
	size_t start;
	size_t delimiter_position;

	delimiter_quoted = remove_quotes_from_field(delimiter);
	start = get_second_line(input->data, 0);
	delimiter_position = get_delimiter_position(input, start, delimiter->value->data);
	if (delimiter_position < input->size)
		string_segment_remove(input, delimiter_position, delimiter->value->size + 1);
	string_init(&content);
	string_set(&content, string_segment_slice(input, start, delimiter_position - start));
	if (!delimiter_quoted)
		expand_parameter_heredoc(&content);
	return (content.data);
}

static char	*generate_random_name_here(void)
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

char *here_document_to_temp_file(char *here_document_content)
{
	char *filename;
	int	fd;

	filename = generate_random_name_here();
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return (filename);
	write(fd, here_document_content, ft_strlen(here_document_content));
	close(fd);
	return (filename);
}


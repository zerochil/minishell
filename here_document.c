/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/14 15:33:07 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_document.h"

char	*generate_random_name(void)
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

#define ERR_HEREDOC_DELIM "minishell: warning: here_document delimited by end-of-file"

bool	is_delimiter(t_string *line, t_string *delimiter)
{
	if (line->size == 1 && string_peek(line) == '\n')
		return (false);
	return (string_match(line, delimiter->data, ft_strcmp, 0));
}

void	expand_parameters(t_string *string)
{
	t_field	field;
	char c;

	string_peek_reset(string);
	field.value = string;
	field.mask = NULL;
	while (true)
	{
		c = string_peek(string);
		if (c == '\0')
			break;
		if (c != '$')
			string_peek_advance(string);
		else
			expand_parameter(&field);
	}
}

bool	was_quoted(t_field *field)
{
	return (field->mask->data[0] != '2');
}
//TODO: test this
//$ env -i ./minishell; cat <<""$HOME 
//		> $PWD
//		> $HOME 
//TODO: no idea what should happen if open/file creation fails.
void	heredoc(char *path, t_field *delimiter)
{
	int		fd;
	char	*line;
	t_string string;

	fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return ;
	string_init(&string);
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf(ERR_HEREDOC_DELIM" (wanted `%s')\n", delimiter->value->data);
			break;
		}
		string_set(&string, line);
		if (is_delimiter(&string, delimiter->value))
			break ;
		if (was_quoted(delimiter))
			expand_parameters(&string);
		write(fd, string.data, string.size);
		write(fd, "\n", 1);
	}
	close(fd);
}

void	handle_heredoc(void *token_ptr)
{
	t_token	*token;
	t_field	*field;
	char	*temp_name;

	token = token_ptr;
	if (token->fields->size != 1)
		return ;
	if (token->type != lexem_get_type("HERE_DOCUMENT"))
		return ;
	field = array_get(token->fields, 0);
	temp_name = generate_random_name();
	heredoc(temp_name, field);
	string_set(field->value, temp_name);
}

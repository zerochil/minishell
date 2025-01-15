/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/15 17:21:00 by inajah           ###   ########.fr       */
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

//TODO: test this
//$ env -i ./minishell; cat <<""$HOME 
//		> $PWD
//		> $HOME 
//TODO: no idea what should happen if open/file creation fails.
void	heredoc(char *path, t_field *delimiter, bool should_expand)
{
	int		fd;
	char	*line;
	t_field *field;

	fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		return ;
	field = field_init("", NULL);
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf(ERR_HEREDOC_DELIM" (wanted `%s')\n", delimiter->value->data);
			break;
		}
		field_set(field, line);
		if (is_delimiter(field->value, delimiter->value))
			break ;
		if (should_expand)
			expand_field_parameter(field, 0);
		write(fd, field->value->data, field->value->size);
		write(fd, "\n", 1);
	}
	close(fd);
}

void	handle_heredoc(void *token_ptr)
{
	t_token	*token;
	t_field	*field;
	char	*temp_name;
	bool	should_expand;

	token = token_ptr;
	if (token->fields->size != 1)
		return ;
	if (token->type != lexem_get_type("HERE_DOCUMENT"))
		return ;
	field = array_get(token->fields, 0);
	temp_name = generate_random_name();
	should_expand = (remove_quotes_from_field(field) == false);
	heredoc(temp_name, field, should_expand);
	string_set(field->value, temp_name);
}

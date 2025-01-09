/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:17:55 by inajah            #+#    #+#             */
/*   Updated: 2025/01/09 20:33:57 by inajah           ###   ########.fr       */
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

#define ERR_HERDOC_DELIM "minishell: warning: here_document delimited by end-of-file"

bool	is_delimiter(t_string *line, t_string *delimiter)
{
	if (line->size == 0)
	{
		printf(ERR_HERDOC_DELIM" wanted (`%s')", delimiter->data);
		return (true);
	}
	if (line->size > delimiter->size)
		return (string_match(delimiter, line->data, 0));
	return (string_match(line, delimiter->data, 0));
}

void	expand_parameters(t_string *string)
{
	(void)string;
	printf("i need expansion please\n");
}

bool	was_quoted(t_token *token)
{
	return (token->mask->data[0] == '2');
}

void	heredoc(char *path, t_token *delimiter)
{
	int		fd;
	char	*line;
	t_string string;

	fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	//TODO: no idea what should happen if this fails.
	if (fd < 0)
		return ;
	string_init(&string);
	while (true)
	{
		line = readline("> ");
		string_set(&string, line);
		if (is_delimiter(&string, delimiter->value))
			break ;
		if (was_qouted(delimiter))
			expand_parameters(&string);
		write(fd, string.data, string.size);
	}
}

void	handle_heredoc(void *token_ptr)
{
	t_token	*token;
	char	*temp_name;

	token = token_ptr;
	if (token->type != lexem_get_type("HERE_DOCUMENT"))
		return ;
	temp_name = generate_random_name();
	heredoc(temp_name, token);
	string_set(token->value, temp_name);
}
	

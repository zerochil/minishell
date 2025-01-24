/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:59:37 by inajah            #+#    #+#             */
/*   Updated: 2025/01/24 09:21:32 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_document.h"

char	*random_filename(void)
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

size_t	get_second_line(char *input, size_t start)
{
	char	in_quote;
	char	c;
	size_t	i;

	in_quote = '\0';
	i = start;
	while (input[i])
	{
		c = input[i];
		if (c == '\0')
			break ;
		if (in_quote == '\0' && (c == '\'' || c == '"'))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (in_quote == '\0' && c == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

void	expand_parameter_here_doc(t_string *content)
{
	char	*parameter_name;
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
				continue ;
		}
		string_peek_advance(content);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_simple_command_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:18:30 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "env.h"

char	**build_arg_list(t_array *fields)
{
	t_field	*field;
	char	**args;
	size_t	i;

	args = track_malloc(sizeof(char *) * (fields->size + 1));
	i = 0;
	while (i < fields->size)
	{
		field = array_get(fields, i);
		args[i] = field->value->data;
		i++;
	}
	args[i] = NULL;
	return (args);
}

char	**get_arg_list(t_array *tokens)
{
	t_token	*token;
	t_array	fields;
	size_t	i;

	array_init(&fields);
	i = 0;
	while (i < tokens->size)
	{
		token = array_get(tokens, i);
		array_merge(&fields, token->fields);
		i++;
	}
	return (build_arg_list(&fields));
}

char	*build_command_path(char *path, char *command_name)
{
	char	*command_path;
	int		len;

	len = ft_strlen(path) + ft_strlen(command_name) + 2;
	command_path = track_malloc(len);
	command_path[0] = '\0';
	ft_strlcat(command_path, path, len);
	ft_strlcat(command_path, "/", len);
	ft_strlcat(command_path, command_name, len);
	return (command_path);
}

char	*get_command_path(char *command_name)
{
	char	*path;
	char	*path_env;
	char	**path_list;
	size_t	i;
	char	*command_path;

	if (command_name == NULL)
		return (NULL);
	if (ft_strchr(command_name, '/') != NULL)
		return (command_name);
	path_env = env_get("PATH");
	if (path_env == NULL)
		return (command_name);
	path_list = ft_split(path_env, ':');
	resource_track(path_list, free_strings);
	i = 0;
	while (path_list[i] != NULL && *command_name != '\0')
	{
		path = path_list[i];
		command_path = build_command_path(path, command_name);
		if (access(command_path, F_OK) == 0)
			return (command_path);
		resource_free(command_path);
		i++;
	}
	return (NULL);
}

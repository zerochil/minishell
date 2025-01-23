/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 06:26:33 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:26:35 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	*env_set_append(char *var, char *key)
{
	t_string	string;
	char		*new_var;

	string_init(&string);
	string_append(&string, key);
	string_append(&string, "=");
	string_append(&string, env_get(key));
	string_append(&string, get_value(var));
	new_var = ft_strdup(string.data);
	string_destroy(&string);
	return (new_var);
}

bool	env_set(char *var)
{
	t_array	*environment;
	char	*key;
	char	*new_var;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	key = get_key(var);
	if (key == NULL)
		return (manager_scope_end(), false);
	if (ft_strchr(var, '=') == NULL && env_get(key) && get_value(var) == NULL)
		return (manager_scope_end(), true);
	if (ft_strchr(var, '=') && ft_strchr(var, '=')[-1] == '+')
		new_var = env_set_append(var, key);
	else
		new_var = ft_strdup(var);
	resource_track(new_var, free);
	env_unset(key);
	free(key);
	array_push(environment, new_var);
	array_sort(environment, compare_strings);
	return (manager_scope_end(), true);
}

bool	env_set_key_value(char *key, char *value)
{
	t_string	string;
	char		*new_var;
	bool		is_set;

	is_set = false;
	string_init(&string);
	string_append(&string, key);
	string_append(&string, "=");
	string_append(&string, value);
	new_var = ft_strdup(string.data);
	string_destroy(&string);
	if (env_set(new_var))
		is_set = true;
	free(new_var);
	return (is_set);
}

void	env_unset(char *key)
{
	t_array	*environment;
	char	*tmp;
	int		i;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	i = array_index_of(environment, key, match_key);
	if (i != -1)
	{
		tmp = array_get(environment, i);
		array_remove(environment, i);
		resource_free(tmp);
	}
	manager_scope_end();
}

char	*env_get(char *key)
{
	t_array	*environment;
	char	*value;

	environment = get_environment_instance();
	value = array_find(environment, key, match_key);
	if (value)
		return (get_value(value));
	return (NULL);
}

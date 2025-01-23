/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 06:26:44 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 17:42:24 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env.h>

char	*get_key(char *var)
{
	int	key_len;

	if (var == NULL)
		return (NULL);
	key_len = ft_strcspn(var, "=");
	if (key_len > 0 && var[key_len] == '=' && var[key_len - 1] == '+')
		key_len--;
	if (is_valid_string(is_valid_identifier, var, key_len) == false)
		return (NULL);
	return (ft_strndup(var, key_len));
}

char	*get_value(char *var)
{
	char	*equal_pos;

	if (var == NULL)
		return (NULL);
	equal_pos = ft_strchr(var, '=');
	if (equal_pos)
		return (equal_pos + 1);
	else
		return (NULL);
}

int	compare_strings(void *a, void *b)
{
	return (ft_strcmp(a, b));
}

int	match_key(void *element_ptr, void *target_ptr)
{
	char	*element;
	char	*target;
	size_t	element_length;
	size_t	target_length;

	element = (char *)element_ptr;
	target = (char *)target_ptr;
	if (ft_strchr(element, '='))
	{
		element_length = ft_strcspn(element, "=");
		target_length = ft_strlen(target);
		if (element_length < target_length)
			element_length = target_length;
		return (ft_strncmp(element, target, element_length) == 0);
	}
	return (ft_strcmp(element, target) == 0);
}

char	**env_get_array(char *program_name)
{
	t_array	*environment;
	char	**env_array;
	char	*env_var;
	size_t	i;

	(void)program_name;
	environment = get_environment_instance();
	env_array = track_malloc((environment->size + 2) * sizeof(char *));
	i = 0;
	array_reset(environment);
	while (true)
	{
		env_var = array_next(environment);
		if (!env_var)
			break ;
		if (ft_strchr(env_var, '='))
		{
			env_array[i] = env_var;
			i++;
		}
	}
	env_array[i] = NULL;
	return (env_array);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:18:37 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 11:52:29 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parameter_expansion.h"

static bool	is_valid_parameter_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*parameter_get(char *parameter_name)
{
	char	*parameter_value;

	if (ft_strcmp(parameter_name, "?") == 0)
		return (ctx_exit_status(CTX_GET, CTX_NO_VALUE));
	parameter_value = env_get(parameter_name);
	if (parameter_value == NULL)
		return ("");
	return (parameter_value);
}

char	*get_parameter_name(t_string *string, size_t dolar_position)
{
	char	*str;
	size_t	i;

	str = string->data;
	i = dolar_position + 1;
	if (!ft_isalpha(str[i]) && str[i] != '_' && str[i] != '?')
	{
		string_peek_advance(string);
		return (NULL);
	}
	while (is_valid_parameter_name_char(str[i]))
		i++;
	if (dolar_position + 1 == i && str[i] == '?')
		i++;
	return (string_segment_extract(string, dolar_position,
			i - dolar_position));
}

int	expand_parameter(t_string *string, char *parameter_name)
{
	char	*parameter_value;
	size_t	value_length;
	size_t	key_length;
	size_t	dolar_position;

	if (parameter_name == NULL)
		return (-1);
	dolar_position = string->peek;
	parameter_value = parameter_get(parameter_name + 1);
	key_length = ft_strlen(parameter_name);
	value_length = ft_strlen(parameter_value);
	string_segment_replace(string, dolar_position,
		key_length, parameter_value);
	string->peek += value_length;
	return (value_length);
}

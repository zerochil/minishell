/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:48:24 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 08:48:30 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	destroy_context(void)
{
	manager_free_everything();
	rl_clear_history();
}

void	clean_exit(int status)
{
	destroy_context();
	exit(status);
}

bool is_directory(char *path)
{
	struct stat path_stat;

	if (stat(path, &path_stat) == -1)
		return (false);
	return (S_ISDIR(path_stat.st_mode));
}

bool is_valid_string(bool is_valid(char, int), char *str, size_t length)
{
	size_t i;

	if (str == NULL || is_valid == NULL || length == 0)
		return (false);
	if (length > ft_strlen(str))
		length = ft_strlen(str);
	i = 0;
	while (i < length)
	{
		if (is_valid(str[i], i) == false)
			return (false);
		i++;
	}
	return (true);
}

bool is_valid_identifier(char c, int i)
{
	if (i == 0)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

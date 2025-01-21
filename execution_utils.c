/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:18:56 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "utils.h"
#include <sys/stat.h>

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

void display_error(char *program_name, char *command, char *error)
{
	ft_putstr_fd(program_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

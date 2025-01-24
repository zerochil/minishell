/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:48:24 by inajah            #+#    #+#             */
/*   Updated: 2025/01/24 17:31:02 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

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

void display_error(char *program_name, char *command, char *error)
{
	ft_putstr_fd(program_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

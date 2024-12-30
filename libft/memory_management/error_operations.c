/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:19:48 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:40:15 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"

void	report_error(char *message)
{
	if (message && *message != '\0')
		ft_putendl_fd(message, 2);
}

void	error(char *message)
{
	if (message && *message != '\0')
		ft_putendl_fd(message, 2);
	manager_free_everything();
	exit(1);
}

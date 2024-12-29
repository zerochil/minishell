/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:46:28 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/10 18:37:04 by rrochd           ###   ########.fr       */
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
	free_all_resources();
	exit(1);
}

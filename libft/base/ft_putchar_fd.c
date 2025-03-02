/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:16:41 by rrochd            #+#    #+#             */
/*   Updated: 2025/03/02 16:49:13 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

bool	ft_putchar_fd(char c, int fd)
{
	if (write(fd, &c, 1) < 0)
	{
		perror("write error");
		return (false);
	}
	return (true);
}

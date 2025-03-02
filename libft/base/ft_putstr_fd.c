/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:29:54 by rrochd            #+#    #+#             */
/*   Updated: 2025/03/02 16:52:26 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

bool	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return (true);
	if (write(fd, s, ft_strlen(s)) < 0)
	{
		perror("write error");
		return (false);
	}
	return (true);
}

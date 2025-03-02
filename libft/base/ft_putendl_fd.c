/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:17:04 by rrochd            #+#    #+#             */
/*   Updated: 2025/03/02 16:44:12 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

bool	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return (true);
	return (ft_putstr_fd(s, fd)
		&& ft_putchar_fd('\n', fd));
}

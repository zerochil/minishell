/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:37:18 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:40 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	ft_isnumber(char *str)
{
	int	is_number;

	is_number = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && ft_isdigit(*str))
	{
		is_number = 1;
		str++;
	}
	while (ft_isspace(*str))
		str++;
	if (*str)
		return (0);
	return (is_number);
}

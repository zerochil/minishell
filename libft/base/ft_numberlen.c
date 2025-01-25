/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numberlen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:37:18 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 12:16:23 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	ft_numberlen(char *str)
{
	int	len;

	len = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str == '0')
		str++;
	while (str[len] && ft_isdigit(str[len]))
	{
		if (INT_MAX - 1 == len)
			return (-1);
		len++;
	}
	while (ft_isspace(str[len]))
		str++;
	if (str[len] != '\0')
		return (-1);
	return (len);
}

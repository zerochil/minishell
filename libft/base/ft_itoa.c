/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 22:33:32 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 18:54:09 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_itoa(int n)
{
	char			*p;
	long			tmp;
	unsigned int	len;
	long			nbr;

	len = (n < 0) + 1;
	nbr = n * ((-2 * (n < 0) + 1));
	tmp = nbr;
	while (tmp > 9)
	{
		tmp /= 10;
		len++;
	}
	p = ft_calloc((len + 1), sizeof(char));
	if (!p)
		return (NULL);
	while (len--)
	{
		p[len] = '0' + nbr % 10;
		nbr /= 10;
	}
	if (n < 0)
		p[0] = '-';
	return (p);
}

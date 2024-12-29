/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:57:46 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last_occurrence;
	char		cc;

	last_occurrence = NULL;
	cc = c;
	while (*s)
	{
		if (*s == cc)
			last_occurrence = s;
		s++;
	}
	if (cc == '\0')
		return ((char *)s);
	return ((char *)last_occurrence);
}

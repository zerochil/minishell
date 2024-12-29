/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 04:26:05 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 09:59:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*string;
	size_t	i;

	if (!s || !f)
		return (NULL);
	string = ft_strdup(s);
	if (!string)
		return (NULL);
	i = 0;
	while (s[i])
	{
		string[i] = f(i, s[i]);
		i++;
	}
	return (string);
}

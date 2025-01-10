/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ends_with.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:21:24 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/09 13:02:33 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ends_with(const char *str, const char *suffix)
{
	size_t	str_len;
	size_t	suffix_len;

	if (str == NULL || suffix == NULL)
		return (1);
	str_len = ft_strlen(str);
	suffix_len = ft_strlen(suffix);
	return (suffix_len <= str_len && ft_strcmp(str + str_len - suffix_len,
			suffix) == 0);
}

int	starts_with(const char *str, const char *prefix)
{
	size_t	prefix_len;

	if (str == NULL || prefix == NULL)
		return (1);
	prefix_len = ft_strlen(prefix);
	return (ft_strncmp(str, prefix, prefix_len) == 0);
}

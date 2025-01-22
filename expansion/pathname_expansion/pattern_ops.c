/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:51:44 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:25:26 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pathname_expansion.h"

char	*pattern_extract_dir_path(t_field *pattern)
{
	char	*dir_path;
	size_t	len;

	dir_path = ".";
	if (ft_strchr(pattern->value->data, '/'))
	{
		len = ft_strrchr(pattern->value->data, '/') - pattern->value->data + 1;
		dir_path = string_segment_slice(pattern->value, 0, len);
		string_segment_remove(pattern->mask, 0, len);
	}
	return (dir_path);
}

static void	double_assign(char **a, char **b, char *c, char *d)
{
	*a = c;
	*b = d;
}

bool	pattern_matches(char *pattern_start, char *mask, char *str)
{
	char	*pattern;
	char	*star;
	char	*ss;

	pattern = pattern_start;
	double_assign(&star, &ss, NULL, NULL);
	while (*str)
	{
		if (*pattern == '*' && mask[pattern - pattern_start] == ORIGINAL)
		{
			while (*(pattern + 1) == '*'
				&& mask[(pattern + 1) - pattern_start] == ORIGINAL)
				pattern++;
			double_assign(&star, &ss, pattern++, str);
		}
		else if (*pattern == *str)
			double_assign(&pattern, &str, pattern + 1, str + 1);
		else if (star)
			double_assign(&pattern, &str, star + 1, ++ss);
		else
			return (0);
	}
	while (*pattern == '*' && mask[pattern - pattern_start] == ORIGINAL)
		pattern++;
	return (!*pattern);
}

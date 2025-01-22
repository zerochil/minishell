/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:01:03 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:25:51 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pathname_expansion.h"

bool	is_matching_pattern(t_field *pattern, char *str)
{
	return (pattern_matches(pattern->value->data,
			pattern->mask->data, str));
}

void	recover_full_path(t_array *list, char *dir_path)
{
	size_t	i;
	t_field	*entry;

	if (!ft_strchr(dir_path, '/'))
		return ;
	i = 0;
	while (i < list->size)
	{
		entry = list->data[i];
		string_insert(entry->value, dir_path, 0);
		string_insert(entry->mask, dir_path, 0);
		i++;
	}
}

void	append_trailing_slash(void	*field_ptr)
{
	t_field	*field;

	field = field_ptr;
	string_append(field->value, "/");
	string_append(field->mask, "/");
	field->mask->data[field->mask->size - 1] = EXPANDED;
}

bool	trim_trailing_slash(t_field *pattern)
{
	t_string	*string;
	ssize_t		i;

	string = pattern->value;
	i = string->size - 1;
	while (i >= 0 && string->data[i] == '/')
		i--;
	if (i == (ssize_t)string->size - 1)
		return (false);
	i++;
	string_segment_remove(pattern->value, i, string->size - i);
	string_segment_remove(pattern->mask, i, string->size - i);
	return (true);
}

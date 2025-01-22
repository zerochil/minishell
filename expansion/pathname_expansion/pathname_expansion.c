/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:41:58 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:19:47 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pathname_expansion.h"

bool	target_entry_type(struct dirent *entry, char entry_type)
{
	if (entry_type == DENTRY_HIDDEN)
		return (starts_with(entry->d_name, "."));
	if (entry_type == DENTRY_DIRECTORY)
		return (!starts_with(entry->d_name, ".") && entry->d_type == DT_DIR);
	if (entry_type == DENTRY_HIDDEN + DENTRY_DIRECTORY)
		return (starts_with(entry->d_name, ".") && entry->d_type == DT_DIR);
	return (!starts_with(entry->d_name, "."));
}

t_field	*get_dentry_field(char *dentry_name)
{
	t_field	*dentry_field;

	dentry_field = field_init(dentry_name, NULL);
	ft_memset(dentry_field->mask->data, EXPANDED,
		dentry_field->mask->size);
	return (dentry_field);
}

void	*list_dentries(t_field *pattern, char *dir_path, int target_type)
{
	struct dirent	*entry;
	t_array			*list;
	DIR				*dir;

	list = track_malloc(sizeof(t_array));
	array_init(list);
	dir = opendir(dir_path);
	if (dir == NULL)
		return (list);
	while (true)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (!target_entry_type(entry, target_type))
			continue ;
		if (is_matching_pattern(pattern, entry->d_name))
			array_push(list, get_dentry_field(entry->d_name));
	}
	closedir(dir);
	return (list);
}

t_array	*list_files(t_field *pattern)
{
	t_array	*found_dentries;
	char	*dir_path;
	int		target_type;

	field_remove_original_quotes(pattern);
	target_type = DENTRY_VISIBLE;
	target_type |= DENTRY_DIRECTORY * trim_trailing_slash(pattern);
	dir_path = pattern_extract_dir_path(pattern);
	target_type |= DENTRY_HIDDEN * (pattern->value->data[0] == '.');
	found_dentries = list_dentries(pattern, dir_path, target_type);
	recover_full_path(found_dentries, dir_path);
	resource_free(pattern);
	if (found_dentries->size == 0)
	{
		resource_free(found_dentries);
		return (NULL);
	}
	if (target_type & DENTRY_DIRECTORY)
		array_do(found_dentries, append_trailing_slash);
	return (found_dentries);
}

t_array	*pathname_expansion_field(t_field *field)
{
	t_array	*filenames;

	if (ft_strchr(field->value->data, '*') == NULL)
		return (NULL);
	filenames = list_files(field_copy(field));
	if (filenames == NULL)
		return (NULL);
	return (filenames);
}

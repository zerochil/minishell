/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:51:06 by inajah            #+#    #+#             */
/*   Updated: 2025/01/28 09:15:11 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATHNAME_EXPANSION_H
# define PATHNAME_EXPANSION_H

# include "field.h"
# include "libft.h"
# include <dirent.h>

enum	e_dentry_type
{
	DENTRY_VISIBLE,
	DENTRY_HIDDEN,
	DENTRY_DIRECTORY,
};

char	*pattern_extract_dir_path(t_field *pattern);
bool	pattern_matches(char *pattern_start, char *mask, char *str);
int		field_compare_strings(void *a_ptr, void *b_ptr);

bool	is_matching_pattern(t_field *pattern, char *str);
void	recover_full_path(t_array *list, char *dir_path);
void	append_trailing_slash(void *field_ptr);
bool	trim_trailing_slash(t_field *pattern);
t_array	*pathname_expansion_field(t_field *field);

#endif

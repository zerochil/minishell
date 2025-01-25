/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 20:00:28 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 06:41:34 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_H
# define STRING_H

# include "../_containers_dependencies.h"
# include <stdbool.h>

typedef struct s_string
{
	char	*data;
	size_t	size;
	size_t	capacity;
	size_t	peek;
}			t_string;

void		string_init(t_string *str);
void		string_set(t_string *string, char *new_text);
void		string_destroy(t_string *str);
void		string_ensure_capacity(t_string *string, size_t needed_capacity);

char		string_peek(t_string *string);
char		string_peek_advance(t_string *string);
char		string_peek_reset(t_string *string);

void		string_append(t_string *str, const char *text);
char		string_shift(t_string *str);
void		string_shift_by(t_string *string, size_t offset);
void		string_shift_while(t_string *string, char *charset);
void		string_insert(t_string *string, const char *text, size_t index);

int			string_match(t_string *string, char *target,
				int (*cmp)(const char *, const char *, size_t), size_t start);

void		string_segment_remove(t_string *string, size_t start,
				size_t length);
void		string_segment_replace(t_string *string, size_t start,
				size_t length, const char *new_text);
char		*string_segment_extract(t_string *string, size_t start,
				size_t length);
char		*string_segment_slice(t_string *string, size_t start,
				size_t length);

void		string_replace(t_string *string, const char *old_text,
				const char *new_text);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/14 09:40:15 by inajah           ###   ########.fr       */
/*   Updated: 2025/01/10 09:57:42 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "env.h"

char	*get_param_value(char *param_name)
{
	char	*param_value;

	param_value = env_get(param_name);
	if (param_value == NULL)
		return ("");
	return (param_value);
}


//////////////////////////////// field helper functions //////////////////////////////////////
void	field_peek(t_field *field, char *c, char *m)
{
	*c = string_peek(field->value);
	if (field->mask)
		*m = string_peek(field->mask);
}

void	field_peek_set(t_field *field, size_t peek)
{
	field->value->peek = peek;
	if (field->mask)
		field->mask->peek = peek;
}

void	field_peek_advance(t_field *field)
{
	string_peek_advance(field->value);
	if (field->mask)
		string_peek_advance(field->mask);
}

void	field_peek_reset(t_field *field)
{
	string_peek_reset(field->value);
	if (field->mask)
		string_peek_reset(field->mask);
}

char	field_shift_at_peek(t_field *field)
{
	char	c;

	c = string_peek(field->value);
	string_segment_remove(field->value, field->value->peek, 1);
	if (field->mask)
		string_segment_remove(field->mask, field->mask->peek, 1);
	return (c);
}

void	field_shift(t_field *field)
{
	string_shift(field->value);
	string_shift(field->mask);
}

t_field	*field_init(char *token_value, char *mask)
{
	t_field	*field;
	size_t	token_value_length;

	if (token_value == NULL)
	{
		report_error("field_init: error");
		token_value = "";
	}
	field = track_malloc(sizeof(t_field));
	field->value = track_malloc(sizeof(t_string));
	string_init(field->value);
	string_set(field->value, token_value);
	field->mask = track_malloc(sizeof(t_string));
	string_init(field->mask);
	if (mask != NULL)
		string_set(field->mask, mask);
	else
	{
		token_value_length = ft_strlen(token_value);
		string_ensure_capacity(field->mask, token_value_length);
		ft_memset(field->mask->data, '0', token_value_length);
		field->mask->size = token_value_length;
	}
	return (field);
}
///////////////////////////////// expansion utility functions ///////////////////////////////////////////////
bool	remove_quotes_from_field(t_field *field)
{
	size_t	old_size;
	char	in_quote;
	char	c;
	char	m;

	old_size = field->value->size;
	in_quote = '\0';
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
			in_quote = field_shift_at_peek(field);
		else if (in_quote == c && m == '0')
			in_quote = field_shift_at_peek(field) != in_quote;
		else
			field_peek_advance(field);
	}
	return (field->value->size != old_size);
}

static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_valid_param_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/////////////////////////////////// parameter expansion //////////////////////////
void	field_replace(t_field *field, size_t start, size_t length,
		char *value)
{
	char	*value_mask;
	size_t	value_length;
	
	if (field->mask == NULL || value == NULL)
	{
		report_error("field_replace: error");
		return ;
	}
	value_length = ft_strlen(value);
	string_segment_replace(field->value, start, length, value);
	value_mask = track_malloc((value_length + 1) * sizeof(char));
	ft_memset(value_mask, '1', value_length);
	string_segment_replace(field->mask, start, length, value_mask);
	resource_free(value_mask);
}

void	expand_parameter(t_field *field)
{
	char	*param_name;
	char	*param_value;
	size_t	start;

	start = field->value->peek;
	string_peek_advance(field->value);
	while (is_valid_param_char(string_peek(field->value)))
		string_peek_advance(field->value);
	if (start + 1 == field->value->peek)
		return ;
	param_name = string_segment_extract(field->value, start,
			field->value->peek - start);
	param_value = get_param_value(param_name + 1);
	field_replace(field, start, field->value->peek - start, param_value);
	field->value->peek = start + ft_strlen(param_value);
}

void	expand_field_parameter(t_field *field)
{
	char c;
	char in_quote;

	in_quote = '\0';
	field_peek_reset(field);
	while (true)
	{
		c = string_peek(field->value);
		if (c == '\0')
			break ;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (c == '$' && in_quote != '\'')
			expand_parameter(field);
		else
			string_peek_advance(field->value);
	}
}
//TODO: what if the variable name starts with a digit?
void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	t_field		*field;

	token = token_ptr;
	if (token->type == lexem_get_type("HERE_DOCUMENT"))
		return ;
	token->fields = track_malloc(sizeof(t_array));
	array_init(token->fields);
	field = field_init(token->value, NULL);
	expand_field_parameter(field);
	array_push(token->fields, field);
}



///////////////////////////////// field splitting ////////////////////////////////
t_field	*field_slice_up_to_peek(t_field *field)
{
	t_field	*new_field;
	char	*value;
	char	*mask;

	if (field->value->peek == 0)
		return (NULL);

	value = string_segment_slice(field->value, 0, field->value->peek);
	mask = string_segment_slice(field->mask, 0, field->mask->peek);
	new_field = field_init(value, mask);
	field_peek_reset(field);
	return (new_field);
}

void	skip_field_splitting_chars(t_field *field)
{
	while (is_whitespace(string_peek(field->value))
		&& string_peek(field->mask) == '1')
		field_shift(field);
}

void extract_field(t_array *fields, t_field *field)
{
	t_field *new_field;

	new_field = field_slice_up_to_peek(field);
	if (new_field)
		array_push(fields, new_field);
	skip_field_splitting_chars(field);
}

void single_field_split(t_array *fields)
{
	t_field	*field;
	char in_quote;
	char c;
	char m;

	field = array_shift(fields);
	field_peek_reset(field);
	in_quote = '\0';
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c && m == '0')
			in_quote = '\0';
		if (is_whitespace(c) && m == '1' && in_quote == '\0')
			extract_field(fields, field);
		else
			field_peek_advance(field);
	}
	if (field->value->size > 0)
		array_push(fields, field);
}

bool	is_field_splitting_required(t_token *token)
{
	t_field *field;
	char	*dolar_pos;
	char	dolar;
	char	c;
	char	m;

	dolar = '\0';
	field = array_get(token->fields, 0);
	dolar_pos = ft_strrchr(field->value->data, '$');
	if (dolar_pos == NULL)
		return (true);
	field_peek_set(field, dolar_pos - field->value->data);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break;
		if (c == '$' && m == '0' )
			dolar = c;
		else if (dolar == '$' && m == '1')
			return (true);
		field_peek_advance(field);
	}
	return (dolar == '\0');

}

void field_splitting(void *token_ptr)
{
	t_token *token;

	token = token_ptr;
	if (is_field_splitting_required(token))
		single_field_split(token->fields);
}

/////////////////////////////// pathname expansion /////////////////////////////////

void	array_merge(t_array *dest, t_array *src)
{
	size_t	i;

	i = 0;
	while (i < src->size)
	{
		array_push(dest, src->data[i]);
		i++;
	}
}


void	pattern_update_mask(t_field *pattern)
{
	char	in_quote;
	char	c;

	in_quote = '\0';
	string_peek_reset(pattern->value);
	while (true)
	{
		c = string_peek(pattern->value);
		if (c == '\0')
			break;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		else if (c == '*' && in_quote != '\0')
			pattern->mask->data[pattern->value->peek] = '2';
		string_peek_advance(pattern->value);
	}
}

t_field	*field_copy(t_field *field)
{
	t_field *copy;

	if (!field)
	{
		report_error("token_copy: error");
		return (NULL);
	}
	copy = field_init(field->value->data, field->mask->data);
	return (copy);
}

char	*pattern_extract_dir_path(t_field *pattern)
{
	char *dir_path;
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

enum {
	DENTRY_VISIBLE,
	DENTRY_HIDDEN,
	DENTRY_DIRECTORY,
};

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
	string_append(field->mask, "1");
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

void double_assign(char **a, char **b, char *c, char *d)
{
    *a = c;
    *b = d;
}

int matches_pattern(char *pattern_start, char *mask, char *str)
{
    char *star;
    char *ss;
	char *pattern;

	pattern = pattern_start;
    double_assign(&star, &ss, NULL, NULL);
    while (*str)
    {	
		if (*pattern == '*' && mask[pattern - pattern_start] != '2')
		{
            while (*(pattern + 1) == '*' && mask[(pattern + 1) - pattern_start] != '2')
                pattern++;
            double_assign(&star, &ss, pattern++, str);
        }
		else if (*pattern == *str)
            double_assign(&pattern, &str, pattern + 1, str + 1);
		else if (star)
            double_assign(&pattern, &str, star + 1, ++ss);
		else
            return 0;
    }
    while (*pattern == '*' && mask[pattern - pattern_start] != '2')
        pattern++;
    return !*pattern;
}

void	*list_dentries(t_field *pattern, char *dir_path, int target_type)
{
	t_array	*list;
	struct dirent *entry;
    DIR *dir;

	list = track_malloc(sizeof(t_array));
	array_init(list);
    dir = opendir(dir_path);
	if (dir == NULL)
		return (list);
	while (true)
    {
        entry = readdir(dir);
        if (entry == NULL)
            break;
		if (!target_entry_type(entry, target_type))
			continue ;
		if (matches_pattern(pattern->value->data,
					pattern->mask->data, entry->d_name))
			array_push(list, field_init(entry->d_name, NULL));
    }
	closedir(dir);
	return (list);
}

t_array *list_files(t_field *pattern)
{
    t_array	*found_dentries;
    char	*dir_path;
	int		target_type;

	pattern_update_mask(pattern);
	remove_quotes_from_field(pattern);
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

t_array	*expand_field_pathname(int	token_type, t_field *field)
{
	t_array	*filenames;

	if (token_type == lexem_get_type("HERE_DOCUMENT"))
		return (NULL);
	if (ft_strchr(field->value->data, '*') == NULL)
		return (NULL);
	filenames = list_files(field_copy(field));
	if (filenames == NULL)
		return (NULL);
	return (filenames);
}

void	array_expand_at(t_array *array, size_t index, t_array *subarray)
{
	ssize_t	i;

	if (!array || !subarray)
	{
		report_error("array_expand_at: error");
		return ;
	}
	array_remove(array, index);
	i = subarray->size - 1;
	while (i >= 0)
	{
		array_insert(array, index, subarray->data[i]);
		i--;
	}
}

void	pathname_expansion(void *token_ptr)
{
	t_token *token;
	t_array	*expansion_list;
	size_t	i;

	token = token_ptr;
	i = 0;
	while (i < token->fields->size)
	{
		expansion_list = expand_field_pathname(token->type, array_get(token->fields, i));
		if (expansion_list)
		{
			array_expand_at(token->fields, i, expansion_list);
			i += expansion_list->size;
		}
		else
			i++;
	}
}
///////////////////////// quote removal //////////////////////////
void single_field_quote_removal(void *field_ptr)
{
	t_field	*field;
	
	field = field_ptr;
	if (remove_quotes_from_field(field))
		field->mask->data[0] = '2';
}

void	quote_removal(void *token_ptr)
{
	t_token *token;

	token = token_ptr;
	array_do(token->fields, single_field_quote_removal);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/19 20:50:15 by inajah           ###   ########.fr       */
/*   Updated: 2025/01/10 09:57:42 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "env.h"

char	*parameter_get(char *parameter_name)
{
	char	*parameter_value;

	if (ft_strcmp(parameter_name, "?") == 0)
		return "EXIT_STATUS";
	parameter_value = env_get(parameter_name);
	if (parameter_value == NULL)
		return ("");
	return (parameter_value);
}


//////////////////////////////// field helper functions //////////////////////////////////////

void	field_peek(t_field *field, char *c, char *m)
{
	if (field == NULL || c == NULL || m == NULL)
	{
		report_error("Error: field_peek");
		return ;
	}
	*c = string_peek(field->value);
	if (field->mask)
		*m = string_peek(field->mask);
}


void	field_peek_set(t_field *field, size_t peek)
{
	if (field == NULL || peek > field->value->size)
	{
		report_error("Error: field_peek_set");
		return ;
	}
	field->value->peek = peek;
	if (field->mask)
		field->mask->peek = peek;
}

void	field_peek_advance(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_peek_advance");
		return ;
	}
	string_peek_advance(field->value);
	if (field->mask)
		string_peek_advance(field->mask);
}

void	field_peek_reset(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_peek");
		return ;
	}
	string_peek_reset(field->value);
	if (field->mask)
		string_peek_reset(field->mask);
}

void	field_set(t_field *field, char *value, unsigned char mask)
{
	if (field == NULL)
	{
		report_error("Error: field_set");
		return ;
	}
	string_set(field->value, value);
	string_set(field->mask, value);
	field_peek_reset(field);
	ft_memset(field->mask->data, mask, field->mask->size);
}

char	field_shift_at_peek(t_field *field)
{
	char	c;

	if (field == NULL)
	{
		report_error("Error: field_shift_at_peek");
		return (-1);
	}
	c = string_peek(field->value);
	string_segment_remove(field->value, field->value->peek, 1);
	if (field->mask)
		string_segment_remove(field->mask, field->mask->peek, 1);
	return (c);
}

void	field_shift(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_shift");
		return ;
	}
	string_shift(field->value);
	string_shift(field->mask);
}


t_string *get_token_value_mask(char *token_value)
{
	t_string	*mask;
	size_t	token_value_length;
	char	in_quote;
	size_t	i;

	token_value_length = ft_strlen(token_value);
	mask = track_malloc(sizeof(t_string));
	string_init(mask);
	string_ensure_capacity(mask, token_value_length);
	mask->size = token_value_length;
	in_quote = '\0';
	i = 0;
	while (token_value[i])
	{
		if (in_quote == '\0' && (token_value[i] == '\'' || token_value[i] == '"'))
			in_quote = token_value[i];
		else if (in_quote == token_value[i])
			in_quote = '\0';
		else if (in_quote == '\'')
			mask->data[i] |= SINGLE_QUOTED;
		else if (in_quote == '"')
			mask->data[i] |= DOUBLE_QUOTED;
		i++;
	}
	return (mask);
}

t_field	*field_init(char *token_value, char *mask)
{
	t_field	*field;

	if (token_value == NULL)
	{
		report_error("field_init: error");
		token_value = "";
	}
	field = track_malloc(sizeof(t_field));
	field->value = track_malloc(sizeof(t_string));
	string_init(field->value);
	string_set(field->value, token_value);
	field->mask = get_token_value_mask(token_value);
	if (mask != NULL)
		ft_memcpy(field->mask->data, mask, field->value->size);
	return (field);
}
///////////////////////////////// expansion utility functions ///////////////////////////////////////////////
bool	remove_quotes_from_field(t_field *field)
{
	size_t	old_size;
	char	c;
	char	m;

	old_size = field->value->size;
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break;
		else if ((c == '\'' || c == '"' ) && m == ORIGINAL)
			field_shift_at_peek(field);
		else
			field_peek_advance(field);
	}
	return (field->value->size != old_size);
}

static bool	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_valid_parameter_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/////////////////////////////////// parameter expansion //////////////////////////
void	field_replace(t_field *field, size_t start, size_t length,
		char *value)
{
	size_t	value_length;

	if (field->mask == NULL || value == NULL)
	{
		report_error("field_replace: error");
		return ;
	}
	string_segment_replace(field->value, start, length, value);
	string_segment_replace(field->mask, start, length, value);
	value_length = ft_strlen(value);
	ft_memset(field->mask->data + start, EXPANDED, value_length);
}

char *get_parameter_name(t_string *string, size_t dolar_position)
{
	char *str;
	size_t i;

	str = string->data;
	i = dolar_position + 1;
	if (!ft_isalpha(str[i]) && str[i] != '_' && str[i] != '?')
	{
		string_peek_advance(string);
		return (NULL);
	}
	while (is_valid_parameter_name_char(str[i]))
		i++;
	if(dolar_position + 1 == i && str[i] == '?')
		i++;
	return (string_segment_extract(string, dolar_position,
					i - dolar_position));
}

int	expand_parameter(t_string *string, char *parameter_name)
{
	char	*parameter_value;
	size_t	value_length;
	size_t	key_length;
	size_t	dolar_position;

	if (parameter_name == NULL)
		return (-1);
	dolar_position = string->peek;
	parameter_value = parameter_get(parameter_name + 1);
	key_length = ft_strlen(parameter_name);
	value_length = ft_strlen(parameter_value);
	string_segment_replace(string, dolar_position, key_length, parameter_value);
	string->peek += value_length;
	return (value_length);
}

void	expand_mask_at_peek(t_string *mask, int key_length, int value_length, int value_mask)
{
	char *tmp;

	tmp = safe_malloc((value_length + 1) * sizeof(char));
	ft_memset(tmp, value_mask, value_length);
	string_segment_replace(mask, mask->peek, key_length, tmp);
	free(tmp);
}

bool	expand_parameter_in_field(t_field *field)
{
	char 	*parameter_name;
	size_t	dolar_position;
	size_t	value_length;
	int		mask;

	dolar_position = field->value->peek;
	parameter_name = get_parameter_name(field->value, dolar_position);
	value_length = expand_parameter(field->value, parameter_name);
	if (value_length < 0)
		return (false);
	mask = EXPANDED | (field->mask->data[dolar_position] & DOUBLE_QUOTED);
	expand_mask_at_peek(field->mask, ft_strlen(parameter_name), value_length, mask);
	//field_peek_set(field, dolar_position + value_length);
	//print_field(field);
	return (true);
}

bool	expand_field_parameter(t_field *field, int single_quoted_flag)
{
	char c;
	char m;
	bool	has_expanded;
	bool	should_field_split;

	should_field_split = false;
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break ;
		if (c == '$' && (m & single_quoted_flag) == 0)
		{
			has_expanded = expand_parameter_in_field(field);
			if ((m & DOUBLE_QUOTED) == 0)
				should_field_split = has_expanded;
			field->mask->peek = field->value->peek;
		}
		else
			field_peek_advance(field);
	}
	return should_field_split;
}
//TODO: what if the variable name starts with a digit?
void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	t_field		*field;

	token = token_ptr;
	field = array_get(token->fields, 0);
	//TODO:  handle this: $"HOME" -> HOME, $ gets consumed if it is not quoted and followed by a quote.
	token->should_field_split = expand_field_parameter(field, SINGLE_QUOTED);
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

void	skip_ifs(t_field *field)
{
	while (is_ifs(string_peek(field->value))
		&& (string_peek(field->mask) == EXPANDED))
		field_shift(field);
}

void single_field_split(t_array *fields)
{
	t_field	*field;
	t_field	*new_field;
	char	c;
	char	m;

	field = array_shift(fields);
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break;
		if (is_ifs(c) && m == EXPANDED)
		{
			new_field = field_slice_up_to_peek(field);
			if (new_field)
				array_push(fields, new_field);
			skip_ifs(field);
		}
		else
			field_peek_advance(field);
	}
	if (field->value->size > 0)
		array_push(fields, field);
}

void field_splitting(void *token_ptr)
{
	t_token *token;

	token = token_ptr;
	if (token->should_field_split)
		single_field_split(token->fields);
}

/////////////////////////////// pathname expansion /////////////////////////////////

t_field	*field_copy(t_field *field)
{
	t_field *copy;

	if (!field)
	{
		report_error("field_copy: error");
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
		if (*pattern == '*' && mask[pattern - pattern_start] == ORIGINAL)
		{
            while (*(pattern + 1) == '*' && mask[(pattern + 1) - pattern_start] == ORIGINAL)
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
    while (*pattern == '*' && mask[pattern - pattern_start] == ORIGINAL)
        pattern++;
    return !*pattern;
}

t_field	*get_dentry_field(char *dentry_name)
{
	t_field	*dentry_field;

	dentry_field = field_init(dentry_name, NULL);
	// we never do pathname expansion inside quotes a.k.a '*' or "*"
	// this means dentry_field mask is always set to only EXPANDED
	ft_memset(dentry_field->mask->data, EXPANDED, dentry_field->mask->size);
	return (dentry_field);
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
			array_push(list, get_dentry_field(entry->d_name));
    }
	closedir(dir);
	return (list);
}

t_array *list_files(t_field *pattern)
{
    t_array	*found_dentries;
    char	*dir_path;
	int		target_type;

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

t_array	*expand_field_pathname(t_field *field)
{
	t_array	*filenames;

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
		expansion_list = expand_field_pathname(array_get(token->fields, i));
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
	remove_quotes_from_field(field);
}

void	quote_removal(void *token_ptr)
{
	t_token *token;

	token = token_ptr;
	array_do(token->fields, single_field_quote_removal);
}


bool	is_export_command(t_token *token)
{
	return (ft_strcmp("export", token->value) == 0);
}

bool	is_assingment_word(t_token *token)
{
	char *key;

	key = get_key(token->value);
	if (key == NULL)
		return (false);
	free(key);
	return (true);
}

void	expand_token(t_token *token, int flag)
{
	if (flag & PARAMETER_EXPANSION)
		parameter_expansion(token);
	if (flag & FIELD_SPLITTING)
		field_splitting(token);
	if (flag & PATHNAME_EXPANSION)
		pathname_expansion(token);
	if (flag & QUOTE_REMOVAL)
		quote_removal(token);
}

void	expansion(t_array *tokens)
{
	t_token	*token;
	int	flag;
	bool	is_export;
	size_t	i;

	i = 0;
	if (tokens->size == 0)
		return ;
	is_export = false;
	while (i < tokens->size)
	{
		token = array_get(tokens, i);
		flag = ALL_EXPANSIONS;
		if (token->type == lexem_get_type("HERE_DOCUMENT"))
			flag = NO_EXPANSIONS;
		else if (token->type == lexem_get_type("WORD") && is_export && is_assingment_word(token))
			flag = PARAMETER_EXPANSION | QUOTE_REMOVAL;
		expand_token(token, flag);
		is_export = is_export_command(token);
		i++;
	}
}

void	handle_expansions(t_ast_node *command)
{
	// TODO: chof lik hadi: $xbs kat woli b7ala derti $x
	if (command->type != AST_SIMPLE_COMMAND && command->type != AST_SUBSHELL)
	{
		error("Error: handle_expansions");
		return ;
	}
	if (command->type == AST_SIMPLE_COMMAND)
		expansion(command->children);
	expansion(command->redirect_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/09 15:16:17 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*get_param_value(char *arg_name)
{
	char	*arg_value;

	arg_value = getenv(arg_name);
	if (arg_value == NULL)
		return ("");
	return (arg_value);
}

void	token_peek(t_token *token, char *c, char *m)
{
	*c = string_peek(token->value);
	*m = string_peek(token->mask);
}

void	token_peek_advance(t_token *token)
{
	string_peek_advance(token->value);
	string_peek_advance(token->mask);
}

void	token_peek_reset(t_token *token)
{
	string_peek_reset(token->value);
	string_peek_reset(token->mask);
}

void	token_segment_remove(t_token *token, size_t start, size_t length)
{
	string_segment_remove(token->value, start, length);
	string_segment_remove(token->mask, start, length);
}

static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_valid_param_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	token_update_mask(t_token *token, size_t start, size_t length,
		size_t value_length)
{
	char	*value_mask;

	value_mask = track_malloc((value_length + 1) * sizeof(char));
	ft_memset(value_mask, '1', value_length);
	string_segment_replace(token->mask, start, length, value_mask);
	resource_free(value_mask);
}

void	token_replace_param(t_token *token)
{
	char	*param_name;
	char	*param_value;
	size_t	value_length;
	size_t	start;

	start = token->value->peek;
	string_peek_advance(token->value);
	while (is_valid_param_char(string_peek(token->value)))
		string_peek_advance(token->value);
	if (start + 1 == token->value->peek)
		return ;
	param_name = string_segment_extract(token->value, start,
			token->value->peek - start);
	param_value = get_param_value(param_name + 1);
	value_length = ft_strlen(param_value);
	string_segment_replace(token->value, start,
		token->value->peek - start, param_value);
	token_update_mask(token, start, token->value->peek - start, value_length);
	token->value->peek = start + value_length;
}

//TODO: what if the variable name starts with a digit?
void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	char		in_quote;
	char		c;

	in_quote = '\0';
	token = token_ptr;
	if (token->type == lexem_get_type("HERE_DOCUMENT"))
		return ;
	token_peek_reset(token);
	while (true)
	{
		c = string_peek(token->value);
		if (c == '\0')
			break ;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (c == '$' && in_quote != '\'')
			token_replace_param(token);
		else
			string_peek_advance(token->value);
	}
}

t_token	*token_slice(t_token *token)
{
	t_token	*field_token;
	char	*value;
	char	*mask;

	if (token->value->peek == 0)
		return (NULL);
	field_token = track_malloc(sizeof(t_token));
	field_token->type = token->type;
	value = string_segment_slice(token->value, 0, token->value->peek);
	mask = string_segment_slice(token->mask, 0, token->mask->peek);
	field_token->value = track_malloc(sizeof(t_string));
	field_token->mask = track_malloc(sizeof(t_string));
	string_init(field_token->value);
	string_set(field_token->value, value);
	string_init(field_token->mask);
	string_set(field_token->mask, mask);
	token_peek_reset(token);
	return (field_token);
}

t_field	*field_init(int type)
{
	t_field	*field;

	field = track_malloc(sizeof(t_field));
	field->type = type;
	field->tokens = track_malloc(sizeof(t_array));
	array_init(field->tokens);
	return (field);
}

void	field_push_token(t_field *field, t_token *token)
{
	t_token	*field_token;

	field_token = token_slice(token);
	if (field_token)
		array_push(field->tokens, field_token);
	while (is_whitespace(string_peek(token->value))
		&& string_peek(token->mask) == '1')
	{
		string_shift(token->value);
		string_shift(token->mask);
	}
}

t_field *field_splitting(t_token *token)
{
	t_field *field;
	char in_quote;
	char c;
	char m;

	field = field_init(token->type);
	token_peek_reset(token);
	in_quote = '\0';
	while (true)
	{
		token_peek(token, &c, &m);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c && m == '0')
			in_quote = '\0';
		if (is_whitespace(c) && m == '1' && in_quote == '\0')
			field_push_token(field, token);
		else
			token_peek_advance(token);
	}
	if (token->value->size > 0)
		array_push(field->tokens, token);
	return (field);
}

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

void	expand_redirect_list(t_ast_node *node, t_array *redirect_list)
{
	t_field		*field;
	size_t		i;

	array_do(node->redirect_list, parameter_expansion);
	i = 0;
	while (i < node->redirect_list->size)
	{
		field = field_splitting(node->redirect_list->data[i]);
		array_push(redirect_list, field->tokens);
		i++;
	}
	node->redirect_list = redirect_list;
}

void	expansion(t_ast_node *node)
{
	t_field		*field;
	t_array		*argument_list;
	t_array		*redirect_list;
	size_t		i;

	redirect_list = track_malloc(sizeof(t_array));
	array_init(redirect_list);
	if (node->type == AST_SUBSHELL)
		return (expand_redirect_list(node, redirect_list));
	argument_list = track_malloc(sizeof(t_array));
	array_init(argument_list);
	array_do(node->children, parameter_expansion);
	i = 0;
	while (i < node->children->size)
	{
		field = field_splitting(node->children->data[i]);
		if (lexem_is_redirection(field->type))
			array_push(redirect_list, field->tokens);
		else
			array_merge(argument_list, field->tokens);
		i++;
	}
	node->children = argument_list;
	node->redirect_list = redirect_list;
}

void token_remove_quotes(void *token_ptr);

void	pattern_update_mask(t_token *pattern)
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

t_token	*token_copy(t_token *token)
{
	t_token *copy;

	if (!token)
	{
		report_error("token_copy: error");
		return (NULL);
	}
	copy = token_init(token->type, token->value->data);
	string_set(copy->mask, token->mask->data);
	return (copy);
}

char	*pattern_extract_dir_path(t_token *pattern)
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
	t_token	*entry;

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

void	append_trailing_slash(void	*token_ptr)
{
	t_token	*token;

	token = token_ptr;
	string_append(token->value, "/");
	string_append(token->mask, "1");
}

bool	trim_trailing_slash(t_token *pattern)
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

void	*list_dentries(t_token *pattern, char *dir_path, int target_type)
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
			array_push(list, token_init(0, entry->d_name));
    }
	closedir(dir);
	return (list);
}

t_array *list_files(t_token *pattern)
{
    t_array	*found_dentries;
    char	*dir_path;
	int		target_type;

	pattern_update_mask(pattern);
	token_remove_quotes(pattern);
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

t_array	*token_expand_pathname(t_token *token)
{
	t_array	*filenames;

	if (ft_strchr(token->value->data, '*') == NULL)
		return (NULL);
	filenames = list_files(token_copy(token));
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

void	expand_pattern_list(t_array *pattern_list)
{
	t_array	*expansion_list;
	size_t	i;

	i = 0;
	while (i < pattern_list->size)
	{
		expansion_list = token_expand_pathname(pattern_list->data[i]);
		if (expansion_list)
		{
			array_expand_at(pattern_list, i, expansion_list);
			i += expansion_list->size;
		}
		else
			i++;
	}
}

void	pathname_expansion(t_ast_node *node)
{
	size_t	i;
	t_array	*redirects;

	expand_pattern_list(node->children);
	i = 0;
	while (i < node->redirect_list->size)
	{
		redirects = node->redirect_list->data[i];
		if (redirects->size == 1)
			expand_pattern_list(redirects);
		i++;
	}
}

void token_remove_quotes(void *token_ptr)
{
	t_token *token;
	char 	in_quote;
	char 	c;
	char	m;

	token = token_ptr;
	token_peek_reset(token);
	in_quote = '\0';
	while (true)
	{
		token_peek(token, &c, &m);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
		{
			in_quote = c;
			token_segment_remove(token, token->value->peek, 1);
		}
		else if (in_quote == c && m == '0')
		{
			in_quote = '\0';
			token_segment_remove(token, token->value->peek, 1);
		}
		else
			token_peek_advance(token);
	}
}

void	quote_removal(t_ast_node *node)
{
	t_array	*tokens;
	size_t	i;

	if (node->type == AST_SIMPLE_COMMAND)
		array_do(node->children, token_remove_quotes);
	i = 0;
	while (i < node->redirect_list->size)
	{
		tokens = node->redirect_list->data[i];
		array_do(tokens, token_remove_quotes);
		i++;
	}
}

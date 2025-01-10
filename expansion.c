/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/07 21:31:43 by inajah           ###   ########.fr       */
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

void double_assign(char **a, char **b, char *c, char *d)
{
    *a = c;
    *b = d;
}

int matches_pattern(char *pattern, char *str)
{
    char *star;
    char *ss;

    double_assign(&star, &ss, NULL, NULL);
    while (*str)
    {
        if (*pattern == *str)
            double_assign(&pattern, &str, pattern + 1, str + 1);
        else if (*pattern == '*')
        {
            while (*(pattern + 1) == '*')
                pattern++;
            double_assign(&star, &ss, pattern++, str);
        }
        else if (star)
            double_assign(&pattern, &str, star + 1, ++ss);
        else
            return 0;
    }
    while (*pattern == '*')
        pattern++;
    return !*pattern;
}

t_array *list_files(char *pattern) {
    t_array *found_files;
    char *dir_path;
    struct dirent *entry;
    DIR *dir;

    dir_path = ".";
    if (ft_strchr(pattern, '/'))
    {
        dir_path = pattern;
        *ft_strrchr(dir_path, '/') = '\0';
        pattern = dir_path + ft_strlen(dir_path) + 1;
    }
    dir = opendir(dir_path);
    // TODO: if dir == NULL??
	if (dir == NULL)
		return NULL;
	found_files = track_malloc(sizeof(t_array));
    array_init(found_files);
    while (true)
    {
        entry = readdir(dir);
        if (entry == NULL)
            break;
		//TODO: track strdup
        if (matches_pattern(pattern, entry->d_name))
            array_push(found_files, ft_strdup(entry->d_name));
    }
    closedir(dir);
	if (found_files->size == 0)
		return (NULL);
    return found_files;
}

t_array	*token_expand_pathname(t_token *token)
{
	t_array	*filenames;
	t_token	*new_token;
	size_t	i;

	if (ft_strchr(token->value->data, '*') == NULL)
		return (NULL);
	//TODO: track strdup
	filenames = list_files(ft_strdup(token->value->data));
	if (filenames == NULL)
		return (NULL);
	i = 0;
	while (i < filenames->size)
	{
		new_token = token_init(token->type, (char *)filenames->data[i]);
		ft_memset(new_token->mask->data, '1', new_token->mask->size);
		filenames->data[i] = new_token;
		i++;
	}
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

//TODO: do not expand if * is quoted;

void	pathname_expansion(t_ast_node *node)
{
	size_t	i;
	t_array *expansion_list;
	t_array	*redirects;

	i = 0;
	while (i < node->children->size)
	{
		expansion_list = token_expand_pathname(node->children->data[i]);
		if (expansion_list)
		{
			array_expand_at(node->children, i, expansion_list);
			i += expansion_list->size;
		}
		else
			i++;
	}
	i = 0;
	while (i < node->redirect_list->size)
	{
		redirects = node->redirect_list->data[i];
		if (redirects->size == 1)
		{
			expansion_list = token_expand_pathname(redirects->data[0]);
			if (expansion_list)
				array_expand_at(redirects, 0, expansion_list);
		}
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

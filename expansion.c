/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 12:25:21 by inajah           ###   ########.fr       */
/*   Updated: 2025/01/10 09:57:42 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"



//////////////////////////////// field helper functions //////////////////////////////////////
///////////////////////////////// expansion utility functions ///////////////////////////////////////////////

void field_splitting(void *token_ptr)
{
	t_token *token;

	token = token_ptr;
	if (token->should_field_split)
		field_split(token->fields);
}

/////////////////////////////// pathname expansion /////////////////////////////////
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
	field_remove_original_quotes(field);
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

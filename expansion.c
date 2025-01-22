/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:50:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:11:19 by inajah           ###   ########.fr       */
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



void	pathname_expansion(void *token_ptr)
{
	t_token *token;
	t_array	*expansion_list;
	size_t	i;

	token = token_ptr;
	i = 0;
	while (i < token->fields->size)
	{
		expansion_list = pathname_expansion_field(array_get(token->fields, i));
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
	if (command->type != AST_SIMPLE_COMMAND && command->type != AST_SUBSHELL)
	{
		error("Error: handle_expansions");
		return ;
	}
	if (command->type == AST_SIMPLE_COMMAND)
		expansion(command->children);
	expansion(command->redirect_list);
}

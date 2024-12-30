/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:12:15 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 20:05:18 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void	lexem_add(char *symbol, char *identifier)
{
	static int	id = 1;
	t_array		*lexems;
	t_lexem		*lexem;

	lexems = lexems_get_instance();
	lexem = track_malloc(sizeof(t_lexem));
	lexem->type = id;
	lexem->symbol = symbol;
	lexem->identifier = identifier;
	lexem->symbol_length = ft_strlen(symbol);
	lexem->identifier_length = ft_strlen(identifier);
	array_push(lexems, lexem);
	id++;
}

static void	lexems_init(t_array *lexems)
{
	array_init(lexems);
	lexem_add("&&", "LOGIC_AND");
	lexem_add("||", "LOGIC_OR");
	lexem_add(">>", "REDIRECTION_APPEND");
	lexem_add("<<", "HERE_DOCUMENT");
	lexem_add(">", "REDIRECTION_TRUNC");
	lexem_add("<", "REDIRECTION_IN");
	lexem_add("(", "OPEN_PARENTHESIS");
	lexem_add(")", "CLOSE_PARENTHESIS");
	lexem_add("\n", "NEWLINE");
	lexem_add("|", "PIPE");
	lexem_add("", "EOF");
}

t_array	*lexems_get_instance(void)
{
	static t_array	*lexems;

	if (lexems == NULL)
	{
		lexems = track_malloc(sizeof(t_array));
		/*track_resource(lexems, lexems_destroyer);*/
		lexems_init(lexems);
	}
	return (lexems);
}

int	lexem_get_type(char *identifier)
{
	t_array	*lexems;
	t_lexem	*lexem;

	lexems = lexems_get_instance();
	lexem = array_find(lexems, identifier, lexem_match_identifier);
	if (lexem == NULL)
		return (0);
	return (lexem->type);
}

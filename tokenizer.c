/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:07 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 20:08:46 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tokenizer.h"

static t_token	*token_init(int type, char *value)
{
	t_token	*token;

	token = track_malloc(sizeof(t_token));
	/*track_transfer_ownership(token, "global");*/
	token->type = type;
	if (lexem_is_redirection(type))
		token->filename = value;
	else if (type == 0)
		token->word = value;
	return (token);
}

static t_token	*tokenize_word(t_string *input)
{
	char	c;
	char	in_quote;
	char	*word;

	in_quote = '\0';
	string_peek_reset(input);
	string_shift_while(input, WHITE_SPACE);
	while (1)
	{
		c = string_peek(input);
		if (in_quote == '\0' && ft_strchr(WHITE_SPACE, c)
			&& array_find(lexems_get_instance(), input, lexem_match_word_break))
			break ;
		else if (in_quote == c)
			in_quote = '\0';
		else if (in_quote == '\0' && (c == '\'' || c == '"'))
			in_quote = c;
		string_peek_advance(input);
	}
	word = string_segment_slice(input, 0, input->peek);
	return (token_init(0, word));
}

static t_token	*tokenize_non_word(t_string *input)
{
	t_array	*lexems;
	t_lexem	*lexem;
	t_token	*token;
	t_token	*next_token;

	lexems = lexems_get_instance();
	token = NULL;
	string_shift_while(input, WHITE_SPACE);
	lexem = array_find(lexems, input, lexem_match_symbol);
	if (lexem == NULL)
		return (NULL);
	string_shift_by(input, lexem->symbol_length);
	if (lexem_is_redirection(lexem->type))
	{
		next_token = tokenize_word(input);
		token = token_init(lexem->type, next_token->word);
	}
	else if (lexem->type != lexem_get_type("EOF"))
		token = token_init(lexem->type, NULL);
	return (token);
}

static t_token	*token_next(t_string *input)
{
	t_token	*token;

	string_peek_reset(input);
	if (string_peek(input) == '\0')
		return (token_init(lexem_get_type("EOF"), NULL));
	token = tokenize_non_word(input);
	if (token == NULL)
		token = tokenize_word(input);
	return (token);
}

t_array	*tokenize(t_string *input)
{
	t_array	*tokens;
	t_token	*token;

	if (!is_quoted(input))
	{
		report_error("tokenizer: unbalanced quotes!");
		return (NULL);
	}
	tokens = track_malloc(sizeof(t_array));
	array_init(tokens);
	while (1)
	{
		token = token_next(input);
		array_push(tokens, token);
		if (token->type == lexem_get_type("EOF"))
			break ;
	}
	return (tokens);
}

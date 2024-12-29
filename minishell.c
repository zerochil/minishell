/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 16:38:55 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include "tokenizer.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	print(void *token_ptr)
{
	t_array	*lexems;
	t_token	*token;
	char	*id;

	lexems = lexems_get_instance();
	token = token_ptr;
	if (token->type)
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	else
		id = "WORD";
	printf("[%s, %s]\n", id, token->filename);
	fflush(NULL);
}

int	is_quoted(t_string *input)
{
	int		balance;
	char	in_quote;
	char	c;

	balance = 0;
	in_quote = '\0';
	while (1)
	{
		c = string_peek_advance(input);
		if (c == '\0')
			break ;
		if (in_quote != '\'' && c == '\\')
			string_peek_advance(input);
		else if (in_quote == '\0' && (c == '"' || c == '\''))
		{
			in_quote = c;
			balance = 1;
		}
		else if (in_quote == c)
		{
			in_quote = '\0';
			balance = 0;
		}
	}
	string_peek_reset(input);
	return (balance == 0);
}

int	main(void)
{
	char		*line;
	t_string	input;
	t_array		tokens;

	string_init(&input);
	while (1)
	{
		line = readline("minishell> ");
		add_history(line);
		string_set(&input, line);
		if (!is_quoted(&input))
			report_error("minishell: unbalanced quotes!");
		tokens = tokenize(&input);
		array_do(&tokens, print);
	}
}

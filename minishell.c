/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 20:17:25 by rrochd           ###   ########.fr       */
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

int	main(void)
{
	char		*line;
	t_string	input;
	t_array		*tokens;

	string_init(&input);
	while (1)
	{
		line = readline("minishell> ");
		add_history(line);
		string_set(&input, line);
		tokens = tokenize(&input);
		if (tokens != NULL)
			array_do(tokens, print);
		free(line);
	}
	rl_clear_history();
	free_all_resources();
}

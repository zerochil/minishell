/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 21:24:46 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "minishell.h"
#include "tokenizer.h"
#include "ast.h"
#include "execution.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

void handler(int sig)
{
	(void)sig;
	wait(NULL);
	write(1, "\n", 1);
    rl_on_new_line(); 
    //rl_replace_line("", 0);
	return;
}

int	main(void)
{
	char			*line;
	t_string		input;
	t_array			*tokens;
	t_array			*list;
	t_context		*context;

	// TODO: reset STDI/O to default in case of: `./minishell < file`;
	// TODO: init all instances in some function? or is there a better design than individual instances?
	get_environment_instance();
	context = get_context_instance();
	string_init(&input);
	setup_signals();
	while (1)
	{
		context->foreground = true;
		line = readline(prompt());
		context->foreground = false;
		if (line == NULL)
		{
			free(line);
			destroy_context();
			exit(0);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		string_set(&input, line);
		tokens = tokenize(&input);
		if (tokens == NULL)
			continue ;
		list = generate_ast(tokens);
		execution(list);
		free(line);
	}
	rl_clear_history();
	manager_free_everything();
}

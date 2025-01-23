/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 10:34:11 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "minishell.h"
#include "tokenizer.h"
#include "ast.h"
#include "execution.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <builtins.h>

int	main(void)
{
	char			*line;
	t_string		input;
	t_array			*tokens;
	t_array			*list;
	int 			status;

	// TODO: heredoc fails, exit status is wrong
	//       ALSO: no newline is shown, after pressing ctrl+c.
	//             In SIGINT handler we can print a newline to fix it.
	//       BUT: if we spawn a new minishell, the newline in parent's signal handler mess it up, there is a conflict there;
	//       PERHAPS: it's the heredoc's signal handler that should print the newline
	// TODO: init all instances in some function? or is there a better design than individual instances?
	get_environment_instance();
	tcgetattr(0, ctx_old_termios(CTX_GET));
	string_init(&input);
	setup_signals();

	while (1)
	{
		ctx_is_foreground(CTX_SET, true);
		line = readline(prompt());
		ctx_is_foreground(CTX_SET, false);
		if (line == NULL)
		{
			free(line);
			status = atoi(ctx_exit_status(CTX_GET, CTX_NO_VALUE));
			destroy_context();
			ft_putendl_fd("exit", STDERR_FILENO);
			exit(status);
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
	//	array_do(list, print);
		execution(list);
		free(line);
	}
	rl_clear_history();
	manager_free_everything();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 09:05:07 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "execution.h"
#include "minishell.h"
#include "tokenizer.h"
#include <builtins.h>
#include <readline/history.h>
#include <readline/readline.h>

char	*get_line(void)
{
	char	*line;

	ctx_is_foreground(CTX_SET, true);
	if (atoi(ctx_exit_status(CTX_GET, CTX_NO_VALUE)) == EXIT_STATUS_SIGINT)
		ft_putchar_fd('\n', STDERR_FILENO);
	line = readline(SHELL_NAME "$ ");
	ctx_is_foreground(CTX_SET, false);
	return (line);
}

void	handle_eof(void)
{
	int	status;

	status = atoi(ctx_exit_status(CTX_GET, CTX_NO_VALUE));
	destroy_context();
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	exit(status);
}

void	execute_input(t_string *input)
{
	t_array	*tokens;
	t_array	*list;

	manager_add("execute_line");
	tokens = tokenize(input);
	if (tokens == NULL)
		return ;
	list = generate_ast(tokens);
	execution(list);
	manager_free();
}

void	init_instances(void)
{
	get_environment_instance();
	lexems_get_instance();
	ctx_exit_status(CTX_SET, 0);
	tcgetattr(0, ctx_old_termios(CTX_GET));
	setup_signals();
}

int	main(void)
{
	char		*line;
	t_string	input;

	init_instances();
	while (1)
	{
		line = get_line();
		string_init(&input);
		string_set(&input, line);
		if (line == NULL)
			handle_eof();
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		free(line);
		execute_input(&input);
		string_destroy(&input);
	}
	destroy_context();
}

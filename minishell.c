/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 09:31:44 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "minishell.h"
#include "tokenizer.h"
#include "ast.h"
#include "execution.h"
#include <readline/history.h>
#include <readline/readline.h>

int	main(void)
{
	char			*line;
	t_string		input;
	t_array			*tokens;
	t_array			*list;
	t_context		*context;
	int				status;

	// TODO: reset STDI/O to default in case of: `./minishell < file`;
	// TODO: init all instances in some function? or is there a better design than individual instances?
	get_environment_instance();
	context = get_context_instance();
	string_init(&input);
	setup_signals();
	if (isatty(0) == 0)
	{
		line = get_next_line(0);
		if (ft_strchr(line, '\n'))
			*ft_strchr(line, '\n') = '\0';
		string_set(&input, line);
		tokens = tokenize(&input);
		list = generate_ast(tokens);
		//	array_do(list, handle_expansions);
		status = execution(list);
		manager_free_everything();
		exit(status);
	}
	while (1)
	{
		context->foreground = true;
		line = readline(prompt());
		context->foreground = false;
		if (line == NULL || ft_strlen(line) == 0)
		{
			// TODO: this is fucking problematic; how to differentiate between ctrl-d and a simple enter?
			free(line);
			destroy_context();
			exit(0);
		}
		// TODO: add command to history if it is different than the previous one.
		add_history(line);
		string_set(&input, line);
		tokens = tokenize(&input);
		if (tokens == NULL)
			continue ;
		//array_do(tokens, print_token);
		list = generate_ast(tokens);
		//array_do(list, handle_expansions);
		//array_do(list, print);
		execution(list);
		free(line);
	}
	rl_clear_history();
	manager_free_everything();
}

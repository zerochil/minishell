/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:16:25 by inajah            #+#    #+#             */
/*   Updated: 2025/01/23 08:47:04 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "expansion.h"
#include "execution.h"
#include "signals.h"
#include "libft/libft.h"

static char	*prompt_user_name(void)
{
	char *user_name;

	user_name = env_get("USER");
	if (user_name == NULL)
		return "I have no name";
	return (user_name);
}

static char	*prompt_cwd(void)
{
	static char path[PATH_MAX];
	char *cwd;

	cwd = getcwd(NULL, 0);
	path[0] = '\0';
	if (cwd == NULL)
		ft_strlcpy(path, ".", sizeof(path));
	else
		ft_strlcpy(path, cwd, sizeof(path));
	free(cwd);
	return (path);
}

static char	*prompt_exit_code(void)
{
	char *exit_code;

	exit_code = parameter_get("?");
	return (exit_code);
}

char	*prompt(void)
{
	static t_string string;

	if (string.data == NULL)
		string_init(&string);
	string_set(&string, NULL);
	string_append(&string, prompt_user_name());
	string_append(&string, ":");
	string_append(&string, prompt_cwd());
	string_append(&string, " [");
	string_append(&string, prompt_exit_code());
	string_append(&string, "]");
	string_append(&string, "$ ");
	return (string.data);
}

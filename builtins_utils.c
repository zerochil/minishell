/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 04:51:42 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 04:51:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

builtin_t	*get_builtins_instance(void)
{
	static builtin_t	builtins[] = {
	{"echo", &builtin_echo},
	{"cd", &builtin_cd},
	{"pwd", &builtin_pwd},
	{"export", &builtin_export},
	{"unset", &builtin_unset},
	{"env", &builtin_env},
	{"exit", &builtin_exit},
	{NULL, NULL}
	};

	return (builtins);
}

char	*pwd(char *command_name)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		cwd = ctx_cwd(CTX_GET, CTX_NO_VALUE);
	if (cwd == NULL)
	{
		display_error(command_name,
			"error retrieving current directory: ...",
			strerror(errno));
		return (NULL);
	}
	ctx_cwd(CTX_SET, cwd);
	return (cwd);
}

bool	is_builtin(char *name)
{
	builtin_t	*builtins;
	int			i;

	builtins = get_builtins_instance();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, name) == 0)
			return (true);
		i++;
	}
	return (false);
}

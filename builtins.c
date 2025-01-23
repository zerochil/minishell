/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 04:44:23 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 04:56:14 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	builtin_exit(char **args, int out_fd)
{
	int	exit_status;

	(void)out_fd;
	if (ft_strarr_len(args) > 2)
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return (BUILTIN_EXIT_MISUSE);
	}
	exit_status = BUILTIN_EXIT_SUCCESS;
	if (args[1])
	{
		if (ft_isnumber(args[1]) == 0 || ft_numberlen(args[1]) > 20)
		{
			ft_putendl_fd("exit: numeric argument required", STDERR_FILENO);
			exit_status = BUILTIN_EXIT_MISUSE;
		}
		else
			exit_status = ft_atoi(args[1]);
	}
	if (ctx_is_child(CTX_GET, CTX_NO_VALUE) == false)
		ft_putendl_fd("exit", STDERR_FILENO);
	destroy_context();
	exit(exit_status);
}

int	builtin_echo(char **args, int out_fd)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = 0;
	while (starts_with(args[i], "-n") && ft_strspn(args[i] + 1,
			"n") == ft_strlen(args[i] + 1))
	{
		newline_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], out_fd);
		if (args[i + 1])
			ft_putchar_fd(' ', out_fd);
		i++;
	}
	if (newline_flag == 0)
		ft_putchar_fd('\n', out_fd);
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_cd(char **args, int out_fd)
{
	char	*dir_path;
	char	*oldpwd;

	(void)out_fd;
	if (ft_strarr_len(args) > 2)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO),
			BUILTIN_EXIT_MISUSE);
	if (args[1])
		dir_path = args[1];
	else
	{
		dir_path = env_get("HOME");
		if (!dir_path)
			return (ft_putendl_fd("cd: HOME not set", STDERR_FILENO),
				BUILTIN_EXIT_ERROR);
		if (*dir_path == '\0')
			return (BUILTIN_EXIT_SUCCESS);
	}
	oldpwd = pwd("cd");
	ctx_cwd(CTX_SET, dir_path);
	env_set_key_value("OLDPWD", oldpwd);
	env_set_key_value("PWD", dir_path);
	if (!oldpwd)
		return (BUILTIN_EXIT_ERROR);
	if (chdir(dir_path) != 0)
		return (perror("cd"), BUILTIN_EXIT_ERROR);
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_pwd(char **args, int out_fd)
{
	char	*cwd;

	(void)args;
	cwd = pwd("pwd");
	// TODO: pwd remembers directory if dirpath isn't found;
	if (!cwd)
		return (BUILTIN_EXIT_ERROR);
	ft_putendl_fd(cwd, out_fd);
	return (BUILTIN_EXIT_SUCCESS);
}

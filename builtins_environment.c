/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_environment.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 04:55:10 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/24 16:17:56 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

int	builtin_export_add(char **args)
{
	int	exit_status;

	exit_status = BUILTIN_EXIT_SUCCESS;
	args++;
	while (*args)
	{
		if (env_set(*args) == false)
		{
			exit_status = BUILTIN_EXIT_ERROR;
			ft_putendl_fd("export: not a valid identifier", STDERR_FILENO);
		}
		args++;
	}
	return (exit_status);
}

int	builtin_export_print(int out_fd)
{
	t_array	*environment;
	char	*env_var;

	environment = get_environment_instance();
	array_reset(environment);
	while (true)
	{
		env_var = array_next(environment);
		if (!env_var)
			break ;
		ft_putstr_fd("declare -x ", out_fd);
		if (ft_strchr(env_var, '='))
		{
			while (*env_var != '=')
				ft_putchar_fd(*env_var++, out_fd);
			ft_putstr_fd("=\"", out_fd);
			ft_putstr_fd(env_var + 1, out_fd);
			ft_putchar_fd('\"', out_fd);
		}
		else
			ft_putstr_fd(env_var, out_fd);
		ft_putchar_fd('\n', out_fd);
	}
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_export(char **args, int out_fd)
{
	if (args[1])
		return (builtin_export_add(args));
	return (builtin_export_print(out_fd));
}

int	builtin_unset(char **args, int out_fd)
{
	int	i;

	(void)out_fd;
	i = 1;
	while (args[i])
	{
		env_unset(args[i]);
		i++;
	}
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_env(char **args, int out_fd)
{
	char	**env_array;

	(void)args;
	env_array = env_get_array(NULL);
	while (*env_array)
	{
		ft_putendl_fd(*env_array, out_fd);
		env_array++;
	}
	resource_free(env_array);
	return (BUILTIN_EXIT_SUCCESS);
}

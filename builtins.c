#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include "utils.h"
#include "env.h"

builtin_t *get_builtins_instance()
{
	static builtin_t builtins[] = {
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

int is_builtin(char *name)
{
	builtin_t *builtins;
	int i;

	builtins = get_builtins_instance();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, name) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_export_add(char **args)
{
	int i;

	i = 1;
	char *equal_pos;
	int identifier_len;
	while (args[i])
	{
		// TODO: check if valid identifier
		if (ft_strchr(args[i], '='))
		{
			equal_pos = ft_strchr(args[i], '=');
		    identifier_len = equal_pos - args[i];
			if (is_valid_string(is_valid_identifier, args[i], identifier_len) == 0)
				ft_putendl_fd("export: not a valid identifier", 2);
			else
				env_set(args[i]);
		}
		else
			env_set(args[i]);
		i++;
	}
	return (0);
}

int builtin_export_print(int out_fd)
{
	t_array *environment;
	char *env_var;

	// to test: export var; export var=;
	environment = get_environment_instance();
	array_reset(environment);
	while (true)
	{
		env_var = array_next(environment);
		if (!env_var)
			break;
		ft_putstr_fd("declare -x ", out_fd);
		if (ft_strchr(env_var, '='))
		{
			while (*env_var != '=')
				ft_putchar_fd(*env_var++, out_fd);
			ft_putstr_fd("=\"", out_fd);
			ft_putstr_fd(env_var + 1, out_fd);
			ft_putchar_fd('\"', out_fd);
		} else
			ft_putstr_fd(env_var, out_fd);
		ft_putchar_fd('\n', out_fd);
	}
	return (0);
}

int	builtin_export(char **args, int out_fd)
{
	if (args[1])
		return (builtin_export_add(args));
	return (builtin_export_print(out_fd));
}

int	builtin_exit(char **args, int out_fd)
{
	(void)out_fd;
	int exit_status;

	if (args[1] && args[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	exit_status = 0;
	if (args[1])
	{
		if (ft_isnumber(args[1]) == 0 || ft_numberlen(args[1]) > 20)
		{
			ft_putendl_fd("exit: numeric argument required", 2);
			exit_status = 2;
		}
		else
			exit_status = ft_atoi(args[1]);
	}
	destroy_context();
	exit(exit_status);
}

int	builtin_echo(char **args, int out_fd)
{
	int i;
	int newline_flag;

	i = 1;
	newline_flag = 0;
	while (starts_with(args[i], "-n") && ft_strspn(args[i]+1, "n") == ft_strlen(args[i]+1))
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
	return (0);
}

int count_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_cd(char **args, int out_fd)
{
	char *dir_path;
	char *env_pwd;

	// should I handle: cd -??
	(void)out_fd;
	if (count_args(args) > 2)
		return (ft_putendl_fd("cd: too many arguments", 2), 1);
	if (args[1])
		dir_path = args[1];
	else
	{
		dir_path = env_get("HOME");
		if (!dir_path)
			return (ft_putendl_fd("cd: HOME not set", 2), 1);
	}
	if (chdir(dir_path) != 0)
		return (perror("cd"), 1);
	env_pwd = ft_strjoin("PWD=", dir_path);
	env_set(env_pwd);
	free(env_pwd);
	return (0);
}

int	builtin_unset(char **args, int out_fd)
{
	int i;

	(void)out_fd;
	i = 1;
	while (args[i])
	{
		env_unset(args[i]);
		i++;
	}
	return (0);
}


int	builtin_pwd(char **args, int out_fd)
{
	char *cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("pwd"), 1); // when will this ever happen?
	ft_putendl_fd(cwd, out_fd);
	free(cwd);
	return (0);
}

int	builtin_env(char **args, int out_fd)
{
	char **env_array;

	// fuck is _=/usr/bin/env ?? is there an env that's not builtin?
	// IN EXECUTION: args, same as envp, are all NULL terminated array of strings
	(void)args;
	env_array = env_get_array();
	while (*env_array)
	{
		ft_putendl_fd(*env_array, out_fd);
		env_array++;
	}
	resource_free(env_array);
	return (0);
}

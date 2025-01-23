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

#include <execution.h>
char *pwd(char *command_name)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		cwd = ctx_cwd(CTX_GET, CTX_NO_VALUE);
	if (cwd == NULL)
	{
		display_error(command_name, ": error retrieving current directory: getcwd: cannot access parent directories: ", strerror(errno));
		return (NULL);
	}
	ctx_cwd(CTX_SET, cwd);
	return (cwd);
}

bool is_builtin(char *name)
{
	builtin_t *builtins;
	int i;

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

bool remove_options_delimeter(char **args)
{
	if (args == NULL || args[0] == NULL)
		return (false);
	if (args[1] && ft_strcmp(args[1], "--") == 0)
	{
		ft_strarr_del(args, 1);
		return (true);
	}
	return (false);
}

int	builtin_export_add(char **args)
{
	int exit_status;

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

int builtin_export_print(int out_fd)
{
	t_array *environment;
	char *env_var;

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
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_export(char **args, int out_fd)
{
	if (args[1])
		return (builtin_export_add(args));
	return (builtin_export_print(out_fd));
}

int	builtin_exit(char **args, int out_fd)
{
	int exit_status;

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
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_cd(char **args, int out_fd)
{
	char *dir_path;
	char *env_pwd;

	// TODO: check builtins leaks
	(void)out_fd;
	if (ft_strarr_len(args) > 2)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO), BUILTIN_EXIT_MISUSE);
	if (args[1])
		dir_path = args[1];
	else
	{
		dir_path = env_get("HOME");
		if (!dir_path)
			return (ft_putendl_fd("cd: HOME not set", STDERR_FILENO), BUILTIN_EXIT_ERROR);
		if (*dir_path == '\0')
			return (BUILTIN_EXIT_SUCCESS);
	}
	if (chdir(dir_path) != 0)
		return (perror("cd"), BUILTIN_EXIT_ERROR); // Again, when will this ever happen?
	char *oldpwd = pwd("cd");
	if (!oldpwd)
		return (BUILTIN_EXIT_ERROR);
	env_pwd = ft_strjoin("OLDPWD=", oldpwd);
	env_set(env_pwd);
	free(env_pwd);
	env_pwd = ft_strjoin("PWD=", dir_path);
	env_set(env_pwd);
	free(env_pwd);
	return (BUILTIN_EXIT_SUCCESS);
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
	return (BUILTIN_EXIT_SUCCESS);
}


int	builtin_pwd(char **args, int out_fd)
{
	char *cwd;

	(void)args;
	cwd = pwd(NULL);
	// TODO: pwd remembers directory if dirpath isn't found;
	if (!cwd)
		return (BUILTIN_EXIT_ERROR); // when will this ever happen?
	ft_putendl_fd(cwd, out_fd);
	return (BUILTIN_EXIT_SUCCESS);
}

int	builtin_env(char **args, int out_fd)
{
	char **env_array;

	(void)args;
	env_array = env_get_array(NULL);
	while (*env_array)
	{
		ft_putendl_fd(*env_array, out_fd);
		env_array++;
	}
	ft_putstr_fd("_=", out_fd);
	ft_putendl_fd(args[0], out_fd);

	resource_free(env_array);
	return (BUILTIN_EXIT_SUCCESS);
}

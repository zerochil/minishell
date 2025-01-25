/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 06:18:44 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/24 18:08:41 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env.h>

#define SHLVL_MAX 999
#define SHLVL_MIN 0
#define SHLVL_DEFAULT 1
#define SHLVL_MAX_ERROR "warning: shell level too high. Resetting to 1"

static void	default_shlvl(t_array *environment)
{
	char	*tmp;
	char	*env_var;
	int		shlvl;

	env_var = array_find(environment, "SHLVL", match_key);
	if (env_var && ft_isnumber(env_var + 6))
	{
		shlvl = ft_atoi(env_var + 6);
		if (shlvl < SHLVL_MIN)
			shlvl = 0;
		else if (shlvl >= SHLVL_MAX)
		{
			report_error(SHLVL_MAX_ERROR);
			shlvl = SHLVL_DEFAULT;
		}
		else
			shlvl += 1;
		tmp = ft_itoa(shlvl);
		env_set_key_value("SHLVL", tmp);
		free(tmp);
	}
	else
		env_set_key_value("SHLVL", "1");
}

static void	default_pwd(t_array *environment)
{
	char	*pwd;

	(void)environment;
	pwd = ctx_cwd(CTX_SET, getcwd(NULL, 0));
	if (pwd == NULL)
	{
		display_error("minishell_init", ERR_GETCWD_NO_PARENT, strerror(errno));
		return ;
	}
	env_set_key_value("PWD", pwd);
}

static void	default_oldpwd(t_array *environment)
{
	char	*env_var;
	char	*path;

	env_var = array_find(environment, "OLDPWD", match_key);
	if (!env_var)
		env_set("OLDPWD");
	else
	{
		path = get_value(env_var);
		if (is_directory(path) == false)
			env_unset("OLDPWD");
	}
}

void	set_default_env_vars(t_array *environment)
{
	default_shlvl(environment);
	default_pwd(environment);
	default_oldpwd(environment);
}

t_array	*get_environment_instance(void)
{
	static t_array	environment;
	extern char		**environ;
	char			**env;
	char			*env_var;

	if (environment.data == NULL)
	{
		manager_scope_begin("environment");
		array_init(&environment);
		env = environ;
		while (*env)
		{
			env_var = ft_strdup(*env);
			resource_track(env_var, free);
			array_push(&environment, env_var);
			env++;
		}
		set_default_env_vars(&environment);
		array_sort(&environment, compare_strings);
		manager_scope_end();
	}
	return (&environment);
}

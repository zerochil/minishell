#include "env.h"

int compare_strings(void *a, void *b)
{
	return (ft_strcmp(a, b));
}

int match_key(void *element_ptr, void *target_ptr)
{
	char *element;
	char *target;
	int equal_sign_index;

	element = (char *)element_ptr;
	target = (char *)target_ptr;
	equal_sign_index = ft_strchr(element, '=') - element;
	if (equal_sign_index < 0)
		return (ft_strcmp(element, target) == 0);
	return (ft_strncmp(element, target, equal_sign_index) == 0);
}

void set_default_env_vars(t_array *environment)
{
	char *tmp;
	char *env_var;

	env_var = array_find(environment, "SHLVL", match_key);
	if (env_var && ft_isnumber(env_var + 6))
	{
		tmp = ft_itoa(ft_atoi(env_var + 6) + 1);
		env_var = ft_strjoin("SHLVL=", tmp);
		env_set(env_var);
		free(tmp);
		free(env_var);
	}
	else
		env_set("SHLVL=1");
	env_var = array_find(environment, "PWD", match_key);
	if (!env_var)
	{
		tmp = getcwd(NULL, 0);
		env_var = ft_strjoin("PWD=", tmp);
		env_set(env_var);
		free(tmp);
		free(env_var);
	}
	env_var = array_find(environment, "OLDPWD", match_key);
	if (!env_var)
		env_set("OLDPWD");
}

t_array *get_environment_instance()
{
	static t_array environment;
	extern char **environ;
	char **env;
	char *env_var;

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

void env_set(char *var)
{
	t_array *environment;
	int i;
	char *tmp;
	char *env_var;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	i = array_index_of(environment, var, match_key);
	if (i != -1)
	{
		tmp = array_get(environment, i);
		array_remove(environment, i);
		resource_free(tmp);
	}
	env_var = ft_strdup(var);
	resource_track(env_var, free);
	array_push(environment, env_var);
	array_sort(environment, compare_strings);
	manager_scope_end();
}

void env_unset(char *var)
{
	t_array *environment;
	char *tmp;
	int i;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	i = array_index_of(environment, var, match_key);
	if (i != -1)
	{
		tmp = array_get(environment, i);
		array_remove(environment, i);
		resource_free(tmp);
	}
	manager_scope_end();
}

char *env_get(char *key)
{
	t_array *environment;
	char *value;

	environment = get_environment_instance();
	value = array_find(environment, key, match_key);
	if (value)
	{
		if (ft_strchr(value, '='))
			return (ft_strchr(value, '=') + 1);
		else
			return (value);
	}
	return (NULL);
}

char **env_get_array()
{
	t_array *environment;
	char **env_array;
	char *env_var;
	size_t i;

	environment = get_environment_instance();
	env_array = track_malloc((environment->size + 1) * sizeof(char *));
	i = 0;
	array_reset(environment);
	while (true)
	{
		env_var = array_next(environment);
		if (!env_var)
			break;
		if (ft_strchr(env_var, '='))
		{
			env_array[i] = env_var;
			i++;
		}
	}
	env_array[i] = NULL;
	return (env_array);
}


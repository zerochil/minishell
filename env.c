#include "env.h"

char *get_key(char *var)
{
	int key_len;

	if (var == NULL)
		return (NULL);
	key_len = ft_strcspn(var, "=");
	if (key_len > 0 && var[key_len] == '=' && var[key_len - 1] == '+')
		key_len--;
	if (is_valid_string(is_valid_identifier, var, key_len) == false)
		return (NULL);
	return (ft_strndup(var, key_len));
}

static char *get_value(char *var)
{
	char *equal_pos;

	if (var == NULL)
		return (NULL);
	equal_pos = ft_strchr(var, '=');
	if (equal_pos)
		return (equal_pos + 1);
	else
		return (NULL);
}


int compare_strings(void *a, void *b)
{
	return (ft_strcmp(a, b));
}

int match_key(void *element_ptr, void *target_ptr)
{
	char *element;
	char *target;
	size_t	element_length;
	size_t	target_length;

	element = (char *)element_ptr;
	target = (char *)target_ptr;
	if (ft_strchr(element, '='))
	{
		element_length = ft_strcspn(element, "=");
		target_length = ft_strlen(target);
		if (element_length < target_length)
			element_length = target_length;
		return (ft_strncmp(element, target, element_length) == 0);
	}
	return (ft_strcmp(element, target) == 0);
}

void set_default_env_vars(t_array *environment)
{
	char *tmp;
	char *env_var;
	int  shlvl;

	env_var = array_find(environment, "SHLVL", match_key);
	if (env_var && ft_isnumber(env_var + 6))
	{
		shlvl = ft_atoi(env_var + 6);
		if (shlvl < 0)
			shlvl = 0;
		else if (shlvl >= 999)
		{
			report_error("minishell: warning: shell level (xxx) too high, resetting to 1");
			shlvl = 1;
		}
		else
			shlvl += 1;
		tmp = ft_itoa(shlvl);
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


char *env_set_append(char *var, char *key)
{
	t_string string;
	char *new_var;

	string_init(&string);
	string_append(&string, key);
	string_append(&string, "=");
	string_append(&string, env_get(key));
	string_append(&string, get_value(var));
	new_var = ft_strdup(string.data);
	string_destroy(&string);
	return (new_var);
}

bool env_set(char *var)
{
	t_array *environment;
	char *key;
	char *new_var;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	key = get_key(var);
	if (key == NULL)
		return (manager_scope_end(), false);
	if (ft_strchr(var, '=') == NULL && env_get(key) && get_value(var) == NULL)
		return (manager_scope_end(), true);
	if(ft_strchr(var, '=') && ft_strchr(var, '=')[-1] == '+')
		new_var = env_set_append(var, key);
	else
		new_var = ft_strdup(var);
	resource_track(new_var, free);
	env_unset(key);
	free(key);
	array_push(environment, new_var);
	array_sort(environment, compare_strings);
	return (manager_scope_end(), true);
}

bool env_set_key_value(char *key, char *value)
{
	t_string string;
	char *new_var;
	bool is_set;

	is_set = false;
	string_init(&string);
	string_append(&string, key);
	string_append(&string, "=");
	string_append(&string, value);
	new_var = ft_strdup(string.data);
	string_destroy(&string);
	if (env_set(new_var))
		is_set = true;
	free(new_var);
	return (is_set);
}

void env_unset(char *key)
{
	t_array *environment;
	char *tmp;
	int i;

	manager_scope_begin("environment");
	environment = get_environment_instance();
	i = array_index_of(environment, key, match_key);
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
		return (get_value(value));
	return (NULL);
}

char **env_get_array(char *program_name)
{
	t_array *environment;
	char **env_array;
	char *env_var;
	size_t i;

	environment = get_environment_instance();
	env_array = track_malloc((environment->size + 2) * sizeof(char *));
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
	if (program_name)
		env_array[i++] = ft_strjoin("_=", program_name);
	env_array[i] = NULL;
	return (env_array);
}


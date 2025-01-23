#ifndef ENV_H
#define ENV_H

#include "libft.h"
#include "utils.h"
#include "context.h"

void set_default_env_vars(t_array *environment);
t_array *get_environment_instance();
bool env_set(char *var);
void env_unset(char *var);
char *env_get(char *key);
bool env_set_key_value(char *key, char *value);

char **env_get_array(char *program_name);

char *get_key(char *var);
int compare_strings(void *a, void *b);
int match_key(void *element_ptr, void *target_ptr);

#endif

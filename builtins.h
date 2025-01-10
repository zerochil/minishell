#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

typedef struct s_builtin
{
	char *name;
	int (*function)(char **, int);
} builtin_t;

int is_builtin(char *name);
int execute_builtin(char **args, int out_fd);

int builtin_cd(char **args, int out_fd);
int builtin_pwd(char **args, int out_fd);
int builtin_export(char **args, int out_fd);
int builtin_unset(char **args, int out_fd);
int	builtin_env(char **args, int out_fd);
int builtin_exit(char **args, int out_fd);
int builtin_echo(char **args, int out_fd);

void env_set(char *var);
void env_unset(char *var);
char *env_get(char *key);
char **env_get_array();

#endif

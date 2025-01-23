#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <env.h>
#include <utils.h>
#include <execution.h>

# define BUILTIN_EXIT_SUCCESS 0
# define BUILTIN_EXIT_ERROR 1
# define BUILTIN_EXIT_MISUSE 2


typedef struct s_builtin
{
	char *name;
	int (*function)(char **, int);
} builtin_t;

bool is_builtin(char *name);

int builtin_cd(char **args, int out_fd);
int builtin_pwd(char **args, int out_fd);
int builtin_export(char **args, int out_fd);
int builtin_unset(char **args, int out_fd);
int	builtin_env(char **args, int out_fd);
int builtin_exit(char **args, int out_fd);
int builtin_echo(char **args, int out_fd);

char	*pwd(char *command_name);

builtin_t *get_builtins_instance();

#endif

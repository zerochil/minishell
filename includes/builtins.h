/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:45:32 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:45:55 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <env.h>
# include <execution.h>
# include <libft.h>
# include <stdlib.h>
# include <unistd.h>
# include <utils.h>

# define BUILTIN_EXIT_SUCCESS 0
# define BUILTIN_EXIT_ERROR 1
# define BUILTIN_EXIT_MISUSE 2
# define BUILTIN_WRITE_FAILED 125
typedef struct s_builtin
{
	char	*name;
	int		(*function)(char **, int);
}			t_builtin;

bool		is_builtin(char *name);

int			builtin_cd(char **args, int out_fd);
int			builtin_pwd(char **args, int out_fd);
int			builtin_export(char **args, int out_fd);
int			builtin_unset(char **args, int out_fd);
int			builtin_env(char **args, int out_fd);
int			builtin_exit(char **args, int out_fd);
int			builtin_echo(char **args, int out_fd);

char		*pwd(char *command_name);

t_builtin	*get_builtins_instance(void);

#endif

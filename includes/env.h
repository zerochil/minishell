/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 06:27:18 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/24 16:11:44 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <context.h>
# include <libft.h>
# include <utils.h>
# include <sys/stat.h>

void	set_default_env_vars(t_array *environment);
t_array	*get_environment_instance(void);
bool	env_set(char *var);
void	env_unset(char *var);
char	*env_get(char *key);
bool	env_set_key_value(char *key, char *value);

char	**env_get_array(char *program_name);

char	*get_key(char *var);
int		compare_strings(void *a, void *b);
int		match_key(void *element_ptr, void *target_ptr);
char	*get_value(char *var);

bool	is_directory(char *path);
bool	is_valid_string(bool is_valid(char, int), char *str, size_t length);
bool	is_valid_identifier(char c, int i);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:48:37 by inajah            #+#    #+#             */
/*   Updated: 2025/01/23 17:10:48 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>

void	destroy_context(void);
void	clean_exit(int status);
bool	is_valid_string(bool is_valid(char, int), char *str, size_t length);
bool	is_valid_identifier(char c, int i);
void	display_error(char *program_name, char *command, char *error);

#endif

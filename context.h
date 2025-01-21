/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 15:37:48 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "libft/libft.h"
# include <termios.h>

typedef struct s_context
{
	bool	foreground;
	int 	exit_status;
	struct termios old_termios;
}			t_context;

t_context	*get_context_instance(void);
void		set_exit_status(int status);
char *get_exit_status(void);
struct termios *get_old_termios(void);

#endif

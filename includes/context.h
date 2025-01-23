/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/22 10:35:13 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include "libft.h"
# include <termios.h>

# define CTX_NO_VALUE 0

typedef enum e_ctx_ops
{
	CTX_SET,
	CTX_GET,
} ctx_operation;

typedef struct s_context
{
	bool	foreground;
	int 	exit_status;
	struct termios old_termios;
	bool	is_child;
	char 	*cwd;
}			t_context;

t_context	*get_ctx_instance(void);


char *ctx_exit_status(ctx_operation op, int status);
struct termios *ctx_old_termios(ctx_operation op);
bool ctx_is_child(ctx_operation op, bool is_child);
bool ctx_is_foreground(ctx_operation op, bool is_foreground);
char *ctx_cwd(ctx_operation op, char *cwd);

#endif

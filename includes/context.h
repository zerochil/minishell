/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:47:36 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <errno.h>
# include <libft.h>
# include <string.h>
# include <termios.h>
# include <token.h>

# define SHELL_NAME "Total"
# define CTX_NO_VALUE 0

typedef enum e_ctx_ops
{
	CTX_SET,
	CTX_GET,
}					t_ctx_operation;

typedef struct s_context
{
	int				foreground;
	int				exit_status;
	struct termios	old_termios;
	bool			is_child;
	char			*cwd;

	t_token			*token;
	t_string		*input;
}					t_context;

t_context			*get_ctx_instance(void);

char				*ctx_exit_status(t_ctx_operation op, int status);
struct termios		*ctx_old_termios(t_ctx_operation op);
bool				ctx_is_child(t_ctx_operation op, bool is_child);
int					ctx_is_foreground(t_ctx_operation op, int is_foreground);
char				*ctx_cwd(t_ctx_operation op, char *cwd);

#endif

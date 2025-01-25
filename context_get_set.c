/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_get_set.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:47:51 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <context.h>

char	*ctx_exit_status(t_ctx_operation op, int status)
{
	t_context	*ctx;
	static int	last_exit;
	static char	*last_exit_str;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
	{
		manager_scope_begin("context");
		if (last_exit_str == NULL || last_exit != ctx->exit_status)
		{
			last_exit = ctx->exit_status;
			if (last_exit_str != NULL)
				resource_free(last_exit_str);
			last_exit_str = ft_itoa(last_exit);
			resource_track(last_exit_str, free);
		}
		manager_scope_end();
		return (last_exit_str);
	}
	if (op == CTX_SET)
		ctx->exit_status = status;
	return (NULL);
}

struct termios	*ctx_old_termios(t_ctx_operation op)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (&ctx->old_termios);
	return (NULL);
}

bool	ctx_is_child(t_ctx_operation op, bool is_child)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (ctx->is_child);
	if (op == CTX_SET)
	{
		ctx->is_child = is_child;
	}
	return (is_child);
}

bool	ctx_is_foreground(t_ctx_operation op, bool is_foreground)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (ctx->foreground);
	if (op == CTX_SET)
		ctx->foreground = is_foreground;
	return (is_foreground);
}

char	*ctx_cwd(t_ctx_operation op, char *cwd)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (ctx->cwd);
	if (op == CTX_SET)
	{
		manager_scope_begin("context");
		if (cwd == NULL)
			return (NULL);
		if (ctx->cwd)
		{
			resource_free(ctx->cwd);
			ctx->cwd = NULL;
		}
		if (cwd && ctx->cwd != cwd)
			resource_track(cwd, free);
		ctx->cwd = cwd;
		manager_scope_end();
	}
	return (cwd);
}

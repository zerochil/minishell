/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 15:37:49 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

t_context	*get_ctx_instance(void)
{
	static t_context	ctx;

	return (&ctx);
}

char *ctx_exit_status(ctx_operation op, int status)
{
	t_context	*ctx;
	static int last_exit;
	static char* last_exit_str;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
	{
		if ( last_exit_str == NULL || last_exit != ctx->exit_status)
		{
			last_exit = ctx->exit_status;
			if (last_exit_str)
				resource_free(last_exit_str);
			last_exit_str = ft_itoa(last_exit);
			resource_track(last_exit_str, free);
		}
		return (last_exit_str);
	}
	if (op == CTX_SET)
		ctx->exit_status = status;
	return (NULL);
}

struct termios *ctx_old_termios(ctx_operation op)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (&ctx->old_termios);
	return (NULL);
}

bool ctx_is_child(ctx_operation op, bool is_child)
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

bool ctx_is_foreground(ctx_operation op, bool is_foreground)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (ctx->foreground);
	if (op == CTX_SET)
		ctx->foreground = is_foreground;
	return (is_foreground);
}

char *ctx_cwd(ctx_operation op, char *cwd)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (op == CTX_GET)
		return (ctx->cwd);
	if (op == CTX_SET)
	{
		resource_free(ctx->cwd);
		resource_track(cwd, free);
		ctx->cwd = cwd;
	}
	return (cwd);
}

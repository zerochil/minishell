/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 15:37:49 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

t_context	*get_context_instance(void)
{
	static t_context	context;

	return (&context);
}

void set_exit_status(int status)
{
	t_context	*context;

	context = get_context_instance();
	context->exit_status = status;
}

char *get_exit_status(void)
{
	t_context	*context;
	static int last_exit;
	static char* last_exit_str;

	context = get_context_instance();
	if (last_exit != context->exit_status || last_exit_str == NULL)
	{
		last_exit = context->exit_status;
		if (last_exit_str)
			resource_free(last_exit_str);
		last_exit_str = ft_itoa(last_exit);
		resource_track(last_exit_str, free);
	}
	return (last_exit_str);
}

struct termios *get_old_termios()
{
	t_context	*context;

	context = get_context_instance();
	return (&context->old_termios);
}

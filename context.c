/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/17 16:29:20 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

t_context *get_context_instance()
{
	static t_context context;

	if (context.pids == NULL)
	{
		context.pids = track_malloc(sizeof(t_array));
		array_init(context.pids);
	}
	return (&context);
}


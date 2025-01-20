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

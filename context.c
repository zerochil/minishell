/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_instance.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 04:40:38 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 04:40:49 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <context.h>

t_context	*get_ctx_instance(void)
{
	static t_context	ctx;

	return (&ctx);
}

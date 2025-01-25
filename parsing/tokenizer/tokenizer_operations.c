/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:51:49 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:51:50 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tokenizer.h>

void	token_consume(void)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	ctx->token = NULL;
}

t_token	*token_peek(void)
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (ctx->token == NULL)
		ctx->token = tokenize_next(ctx->input);
	return (ctx->token);
}

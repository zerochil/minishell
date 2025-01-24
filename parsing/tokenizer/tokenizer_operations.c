#include <tokenizer.h>

void	token_consume()
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	ctx->token = NULL;
}

t_token *token_peek()
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	if (ctx->token == NULL)
		ctx->token = tokenize_next(ctx->input);
	return (ctx->token);
}

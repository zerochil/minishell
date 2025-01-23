#include <tokenizer.h>

t_token *token_next()
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	ctx->token = tokenize_next(ctx->input);
	return (ctx->token);
}

t_token *token_peek()
{
	t_context	*ctx;

	ctx = get_ctx_instance();
	return (ctx->token);
}

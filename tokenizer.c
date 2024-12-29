#include "tokenizer.h"

t_token *create_token(const char *value, t_token_type type)
{
	t_token *token;

	token = track_malloc(sizeof(t_token));
	token->value = value;
	token->type = type;
	return (token);
}

t_token *tokenize_next_operator(t_tokenizer *tokenizer)
{
	const t_operator *op;

	op = tokenizer->operators;
	while(op->value != NULL)
	{
		if (ft_strncmp(tokenizer->input.data, op->value, op->length) == 0)
		{
			string_remove_segment(&tokenizer->input, 0, op->length);
			return (create_token(op->value, op->type));
		}
		op++;
	}
	return NULL;
}

int is_operator(const char *input, t_tokenizer *tokenizer)
{
	const t_operator *op;

	op = tokenizer->operators;
	while(op->value != NULL)
	{
		if (ft_strncmp(input, op->value, op->length) == 0)
			return (1);
		op++;
	}
	return (0);
}

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v');
}

void handle_escape(t_string *input, const char in_quote, int pos, char to_escape)
{
	if (in_quote == '"')
	{
		if (to_escape == '\\')
			string_replace_segment(input, pos, 2, "\\");
		else if (to_escape == '"')
			string_replace_segment(input, pos, 2, "\"");
		else if (to_escape == '`')
			string_replace_segment(input, pos, 2, "`");
	}
	else if (in_quote == '\0')
		string_remove_segment(input, pos, 1);
}

t_token *tokenize_next_word(const char *input, t_tokenizer *tokenizer)
{
	char c;
	char in_quote;
	int pos;

	in_quote = '\0';
	pos = 0;
	while (*(input+pos))
	{
		c = input[pos];
		if (in_quote == '\0' && (is_space(c) || is_operator(input+pos, tokenizer)))
			break;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		else if (in_quote != '\'' && c == '\\')
			handle_escape(&tokenizer->input, in_quote, pos, *(input+pos+1));
		pos++;
	}
	// TODO: report error on unclosed quotes
	return (create_token(string_slice_segment(&tokenizer->input, 0, pos), TOKEN_WORD));
}

t_token *tokenize_next(t_tokenizer *tokenizer)
{
	t_token *token;

	if (tokenizer->input.size == 0)
        return create_token(NULL, TOKEN_EOF);
	token = tokenize_next_operator(tokenizer);
	if (token)
		return (token);
	return tokenize_next_word(tokenizer->input.data, tokenizer);
}

void init_tokenizer(t_tokenizer *tokenizer, const char *input)
{
	// TODO: the operators[] array and t_token_type enum are a recipe for disaster
	// you got two sources of truth for token type; modifying one doesn't modify the other; 
	// there got to be a better way of handling this;
	static t_operator operators[] =  {
		{.value = "||", .length = 0, .type = TOKEN_OR},
		{.value = "|",  .length = 0, .type = TOKEN_PIPE},
		{.value = "&&", .length = 0, .type = TOKEN_AND},
		{.value = ">>", .length = 0, .type = TOKEN_REDIR_APPEND},
		{.value = ">",  .length = 0, .type = TOKEN_REDIR_OUT},
		{.value = "<<", .length = 0, .type = TOKEN_REDIR_HEREDOC},
		{.value = "<",  .length = 0, .type = TOKEN_REDIR_IN},
		{.value = ";",  .length = 0, .type = TOKEN_SEMICOLON},
		{.value = "\n", .length = 0, .type = TOKEN_NEWLINE},
		{.value = NULL, .length = 0, .type = 0}
	};
	size_t i;

	i = 0;
	while (i < sizeof(operators) / sizeof(t_operator))
	{
		operators[i].length = ft_strlen(operators[i].value);
		i++;
	}
	tokenizer->operators = operators;
	array_init(&tokenizer->tokens);
	string_init(&tokenizer->input);
	string_append(&tokenizer->input, input);
}

void skip_whitespaces(t_string *input)
{
	while (is_space(string_peek(input)))
		string_shift(input);
}

t_tokenizer tokenize(const char *input)
{
	t_tokenizer tokenizer;
	t_token *token;

	init_tokenizer(&tokenizer, input);
	while (1)
	{
		skip_whitespaces(&tokenizer.input);
		token = tokenize_next(&tokenizer);
		array_push(&tokenizer.tokens, token);
		if (token == NULL || token->type == TOKEN_EOF)
			break;
	}
	return tokenizer;
}

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_NEWLINE,
	TOKEN_EOF,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	const char		*value;
}			t_token;

typedef struct s_operator
{
	const char *value;
	size_t length;
	t_token_type type;
} t_operator;

typedef struct s_tokenizer
{
	t_array		tokens;
	t_string	input;
	t_operator	*operators;
}			t_tokenizer;

t_tokenizer tokenize(const char *input);

#endif


#include <ast.h>

bool is_not_valid_redirection(t_token *token)
{
	if (lexem_is_word(token->type) || ( lexem_is_redirection(token->type) && token->value == NULL))
	{
		syntax_error(ERR_UNEXPECTED);
		return (true);
	}
	return (false);
}

bool	is_filename_missing(t_token *token)
{
	if (token->value == NULL)
	{
		syntax_error(ERR_MISSING_FILENAME);
		return (true);
	}
	return (false);
}

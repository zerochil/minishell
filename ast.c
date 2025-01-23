#include <ast.h>
#include <tokenizer.h>


char	*syntax_error(char *message)
{
	static char	*error_message;

	if (message == NULL)
		error_message = NULL;
	else if (*message != '\0')
		error_message = message;
	return (error_message);
}

bool	check_syntax_error(bool condition, char *message)
{
	if (condition)
	{
		syntax_error(message);
		return (true);
	}
	return (false);
}

t_ast_node	*create_ast_node(t_array *children, t_ast_type type)
{
	t_ast_node	*node;

	node = track_malloc(sizeof(t_ast_node));
	node->type = type;
	node->children = children;
	node->redirect_list = NULL;
	node->error_message = "";
	return (node);
}

t_array *consume_until(t_array *tokens, bool is_target(int), bool to_skip(int), bool is_error(t_token *))
{
	t_array	*consumed;
	t_token	*token;
	size_t	index;

	consumed = array_create();
	index = 0;
	while (index < tokens->size)
	{
		token = array_get(tokens, index);
		if (token == NULL)
			break ;
		if (is_target(token->type))
		{
			token = array_remove(tokens, index);
			if (is_error && is_error(token))
				return (NULL);
			array_push(consumed, token);
		}
		else if (to_skip && to_skip(token->type))
			index++;
		else
			break ;
	}
	return (consumed);
}

bool is_filename_missing(t_token *token)
{
	if (token->value == NULL)
	{
		syntax_error(ERR_MISSING_FILENAME);
		return (true);
	}
	return (false);
}

t_array	*complete_command(t_array *tokens)
{
	skip_linebreak(tokens);
	return (command_list(tokens));
}






// ================<< COMPOND COMMAND >>================

bool is_compound_terminated(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == LEXEM_EOF || token->type == lexem_get_type("NEWLINE"));
}

void skip_line(t_array *tokens)
{
	while (is_compound_terminated(tokens) == false)
		array_shift(tokens);
}

bool check_compound_termination(t_ast_node *compound_node, t_array *tokens)
{
	if (compound_node != NULL && is_compound_terminated(tokens))
		return (true);
	return (false);
}

void push_error_node(t_array *ast_list, t_array *tokens)
{
	t_ast_node	*error_node;
	char *error_message;

	error_node = create_ast_node(NULL, AST_INVALID_COMMAND);
	error_message = syntax_error("");
	if (error_message == NULL)
		error_message = syntax_error("syntax error near unexpected token");
	error_node->error_message = error_message;
	array_push(ast_list, error_node);
	skip_line(tokens);
}

bool is_EOF(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == LEXEM_EOF);
}

t_array	*command_list(t_array *tokens)
{
	t_array		*compound_list;
	t_ast_node	*compound_node;

	compound_list = array_create();
	while (is_EOF(tokens) == false)
	{
		syntax_error(NULL);
		compound_node = compound_command(tokens);
		if (check_compound_termination(compound_node, tokens) == false)
			push_error_node(compound_list, tokens);
		else
			array_push(compound_list, compound_node);
		if (skip_linebreak(tokens) == false)
			break;
	}
	return (compound_list);
}


// ================<< COMPOND COMMAND >>================

int	is_logical_and_or(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("LOGIC_AND"))
		return (AST_BINARY_AND);
	if (token->type == lexem_get_type("LOGIC_OR"))
		return (AST_BINARY_OR);
	return (-1);
}

t_ast_node	*compound_command(t_array *tokens)
{
	t_ast_node	*pipes;
	t_array		*compounds;
	int			type;

	compounds = array_create();
	while (true)
	{
		pipes = pipeline(tokens);
		if (pipes == NULL)
			return (NULL);
		array_push(compounds, pipes);
		type = is_logical_and_or(tokens);
		if (type != -1)
		{
			array_push(compounds, create_ast_node(NULL, type));
			array_shift(tokens);
			skip_linebreak(tokens);
		}
		else
			break ;
	}
	return (create_ast_node(compounds, AST_COMPOUND_COMMAND));
}

t_ast_node	*pipeline(t_array *tokens)
{
	t_token		*token;
	t_array		*pipes;
	t_ast_node	*command_node;

	pipes = array_create();
	while (true)
	{
		command_node = command(tokens);
		if (command_node == NULL)
			return (NULL);
		array_push(pipes, command_node);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("PIPE"))
			array_shift(tokens);
		else
			break ;
		skip_linebreak(tokens);
	}
	return (create_ast_node(pipes, AST_PIPELINE));
}

t_ast_node	*command(t_array *tokens)
{
	t_token	*token;
	t_ast_node	*subshell_node;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("OPEN_PARENTHESIS"))
	{
		array_shift(tokens);
		subshell_node = subshell(tokens);
		return (subshell_node);
	}
	return (simple_command(tokens));
}

t_ast_node	*subshell(t_array *tokens)
{
	t_array	*compound_list;
	t_ast_node	*compound_node;
	t_token		*token;
	t_ast_node	*subshell_node;

	compound_list = array_create();
	while (true)
	{
		skip_linebreak(tokens);
		compound_node = compound_command(tokens);
		if (compound_node == NULL)
			return (NULL);
		array_push(compound_list, compound_node);
		skip_linebreak(tokens);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			array_shift(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			break ;
	}
	if (compound_list->size == 0)
		return (syntax_error(ERR_EMPTY_COMMAND), NULL);
	subshell_node = create_ast_node(compound_list, AST_SUBSHELL);
	subshell_node->redirect_list = consume_until(tokens, lexem_is_redirection, NULL, is_filename_missing);
	return (subshell_node);
}

t_ast_node	*simple_command(t_array *tokens)
{
	t_ast_node	*simple_command_node;
	t_array		*argument_list;
	t_array		*redirections;

	argument_list = consume_until(tokens, lexem_is_word, lexem_is_redirection, NULL);
	redirections = consume_until(tokens, lexem_is_redirection, lexem_is_word, is_filename_missing);
	if (redirections == NULL || argument_list == NULL)
		return NULL;
	if (check_syntax_error(argument_list->size == 0 && redirections->size == 0, ERR_WORD_OR_REDIR))
		return NULL;
	simple_command_node = create_ast_node(argument_list, AST_SIMPLE_COMMAND);
	simple_command_node->redirect_list = redirections;
	return (simple_command_node);
}

bool	skip_linebreak(t_array *tokens)
{
	t_token	*token;
	bool found;

	found = false;
	while (true)
	{
		token = array_peek(tokens);
		if (token->type != lexem_get_type("NEWLINE"))
			break;
		array_shift(tokens);
		found = true;
	}
	return (found);
}

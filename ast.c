#include "ast.h"
#include "tokenizer.h"
#include "debug.h"

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

//TODO: make command list work

/*t_array *generate_ast(t_array *tokens)*/
/*{*/
/*	t_array		*ast_list;*/
/*	t_ast_node	*node;*/
/*	t_token		*token;*/
/*	int			newline_type;*/
/**/
/*	newline_type = lexem_get_type("NEWLINE");*/
/*	ast_list = track_malloc(sizeof(t_array));*/
/*	array_init(ast_list);*/
/*	while (true)*/
/*	{*/
/*		syntax_error(NULL);*/
/*		node = complete_command(tokens);*/
/*		token = array_peek(tokens);*/
/*		if (token->type != -1 && token->type != newline_type)*/
/*		{*/
/*			// skip_line(tokens, node);*/
/*			node->error_message = syntax_error("");*/
/*			node->children = NULL;*/
/*			if (node->error_message == NULL)*/
/*				node->error_message = syntax_error("syntax error near unexpected token");*/
/*			while (token->type != -1 && token->type != newline_type)*/
/*			{*/
/*				array_shift(tokens);*/
/*				token = array_peek(tokens);*/
/*			}*/
/*		}*/
/*		array_push(ast_list, node);*/
/*		linebreak(tokens);*/
/*		token = array_peek(tokens);*/
/*		if (token->type == -1)*/
/*			break ;*/
/*	}*/
/*	return (ast_list);*/
/*}*/



t_array	*complete_command(t_array *tokens)
{
	//TODO: set exit status for syntax error
	linebreak(tokens);
	return (command_list(tokens));
}






// ================<< COMPOND COMMAND >>================

char *set_error_message()
{
	char *error_message;

	error_message = syntax_error("");
	if (error_message == NULL)
		error_message = syntax_error("syntax error near unexpected token");
	return (error_message);
}

bool is_compound_terminated(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == -1 || token->type == lexem_get_type("NEWLINE"));
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

	error_node = create_ast_node(NULL, AST_INVALID_COMMAND);
	error_node->error_message = set_error_message();
	array_push(ast_list, error_node);
	skip_line(tokens);
}

bool is_EOF(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	return (token->type == -1);
}

t_array	*command_list(t_array *tokens)
{
	t_array		*compound_list;
	t_ast_node	*compound_node;

	// TODO: rewrite the top 3 functions in recursive top down;
	// TODO: reset error messages
	compound_list = track_malloc(sizeof(t_array));
	array_init(compound_list);
	while (true)
	{
		compound_node = compound_command(tokens);
		if (check_compound_termination(compound_node, tokens) == false)
			push_error_node(compound_list, tokens);
		else
			array_push(compound_list, compound_node);
		if (linebreak(tokens) == false)
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

	compounds = track_malloc(sizeof(t_array));
	array_init(compounds);
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
			linebreak(tokens);
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

	pipes = track_malloc(sizeof(t_array));
	array_init(pipes);
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
		linebreak(tokens);
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
		token = array_peek(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			array_shift(tokens);
		else
			return (syntax_error(ERR_CLOSE_PARENTHESIS), NULL);
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

	compound_list = track_malloc(sizeof(t_array));
	array_init(compound_list);
	while (true)
	{
		linebreak(tokens);
		compound_node = compound_command(tokens);
		if (compound_node == NULL)
			return (NULL);
		array_push(compound_list, compound_node);
		linebreak(tokens);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
			break ;
		/*if (check_compound_termination(compound_node, tokens) == false)*/
		/*	return (NULL);*/
	}
	if (compound_list->size == 0)
		return (syntax_error(ERR_EMPTY_COMMAND), NULL);
	subshell_node = create_ast_node(compound_list, AST_SUBSHELL);
	subshell_node->redirect_list = redirect_list(tokens);
	return (subshell_node);
}

char	*get_token_symbol(int type)
{
	if (type == lexem_get_type("WORD"))
		return ("WORD");
	if (type == lexem_get_type("REDIRECTION"))
		return ("REDIRECTION");
	if (type == lexem_get_type("PIPE"))
		return "PIPE";
	if (type == lexem_get_type("LOGIC_AND"))
		return "LOGIC_AND";
	if (type == lexem_get_type("LOGIC_OR"))
		return "LOGIC_OR";
	if (type == lexem_get_type("OPEN_PARENTHESIS"))
		return "OPEN_PARENTHESIS";
	if (type == lexem_get_type("CLOSE_PARENTHESIS"))
		return "CLOSE_PARENTHESIS";
	if (type == lexem_get_type("NEWLINE"))
		return "NEWLINE";
	if (type == -1)
		return "EOF";
	return "WORD";
}

t_ast_node	*simple_command(t_array *tokens)
{
	t_ast_node	*simple_command_node;
	t_array		*argument_list;
	t_array		*redirections;
	t_token		*token;

	argument_list = track_malloc(sizeof(t_array));
	array_init(argument_list);
	redirections = track_malloc(sizeof(t_array));
	array_init(redirections);
	while (true)
	{
		token = array_peek(tokens);
		if (!lexem_is_redirection(token->type)
			&& token->type != lexem_get_type("WORD"))
			break ;
		if (lexem_is_redirection(token->type))
		{
			if (check_syntax_error(token->value == NULL, ERR_MISSING_FILENAME))
				return NULL;
			array_push(redirections, token);
		}
		else
			array_push(argument_list, token);
		array_shift(tokens);
	}
	if (check_syntax_error(argument_list->size == 0 && redirections->size == 0,
			ERR_WORD_OR_REDIR))
		return NULL;
	simple_command_node = create_ast_node(argument_list, AST_SIMPLE_COMMAND);
	simple_command_node->redirect_list = redirections;
	return (simple_command_node);
}

t_array	*redirect_list(t_array *tokens)
{
	t_token	*token;
	t_array	*redirections;

	redirections = track_malloc(sizeof(t_array));
	array_init(redirections);
	while (true)
	{
		token = array_peek(tokens);
		if (lexem_is_redirection(token->type) == false)
			break ;
		if (check_syntax_error(token->value == NULL, ERR_MISSING_FILENAME))
			return NULL;
		array_push(redirections, token);
		array_shift(tokens);
	}
	return (redirections);
}

t_token	*io_redirect(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	if (lexem_is_redirection(token->type))
	{
		if (token->value == NULL)
		{
			syntax_error("Missing filename");
			return NULL;
		}
		return (array_shift(tokens));
	}
	return (NULL);
}

bool	linebreak(t_array *tokens)
{
	t_token	*token;

	token = array_peek(tokens);
	// TODO: REFACTOR THIS PATTERN: peek > if token > shift > push to array;
	if (token->type == lexem_get_type("NEWLINE"))
	{
		array_shift(tokens);
		linebreak(tokens);
		return (true);
	}
	return (false);
}

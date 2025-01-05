#include "ast.h"
#include "libft/containers/array/array.h"
#include "libft/memory_management/memory_management.h"
#include "tokenizer.h"

#include "minishell.h"

char *syntax_error(char *message)
{
	static char *error_message;
	if (*message != '\0')
		error_message = message;
	return (error_message);
}

bool	check_syntax_error(bool condition, char *message)
{
	if (condition)
	{
		syntax_error(message);
		return true;
	}
	return false;
}

t_ast_node *create_ast_node(t_array *array, t_ast_type type)
{
	t_ast_node *node;

	node = track_malloc(sizeof(t_ast_node));
	node->type = type;
	node->children = array;
	node->redirect_list = NULL;
	node->error_message = "";
	return node;
}

t_array *generate_ast(t_array *tokens)
{
	t_array *ast_list;
	t_ast_node *node;
	t_token *token;
	int		newline_type;	

	newline_type = lexem_get_type("NEWLINE");
	ast_list = track_malloc(sizeof(t_array));
	array_init(ast_list);
	while(true)
	{
		node = complete_command(tokens);

		token = array_peek(tokens);
		if (token->type != -1 && token->type != newline_type)
		{
			node->error_message = syntax_error("");
			while (token->type != -1 && token->type != newline_type)
			{
				array_shift(tokens);
				token = array_peek(tokens);
			}
		}
		array_push(ast_list, node);
		if (token->type == -1)
			break;
	}
	return ast_list;
}


t_ast_node *complete_command(t_array *tokens)
{
	t_ast_node	*command_list_node;
	t_ast_node	*node;
	/*t_array		*ast_list;*/

	command_list_node = command_list(tokens);
	if (command_list_node == NULL)
	{
		node = create_ast_node(NULL, AST_COMPLETE_COMMAND);
		node->error_message = syntax_error("");
		return node;
	}
	command_list_node->type = AST_COMPLETE_COMMAND;
	return command_list_node;
}

t_ast_node *command_list(t_array *tokens)
{
	t_array	*commands;
	t_ast_node	*command;

	commands = track_malloc(sizeof(t_array));
	array_init(commands);
	linebreak(tokens);
	command = compound_command(tokens);
	if (command == NULL)
		return NULL;
	array_push(commands, command);
	return (create_ast_node(commands, AST_COMMAND_LIST));
}


int is_logical_and_or(t_token *token)
{
	if (token->type == lexem_get_type("LOGIC_AND"))
		return AST_BINARY_AND;
	if (token->type == lexem_get_type("LOGIC_OR"))
		return AST_BINARY_OR;
	return -1;
}

t_ast_node *compound_command(t_array *tokens)
{
	t_token *token;
	t_ast_node	*pipes;
	t_array *compounds;	
	int type;

	compounds = track_malloc(sizeof(t_array));
	array_init(compounds);
	while(true)
	{
		pipes = pipeline(tokens);
		if (pipes == NULL)
			return (NULL);
		array_push(compounds, pipes);
		token = array_peek(tokens);
		type = is_logical_and_or(token);
		if (type != -1)
		{
			array_push(compounds, create_ast_node(NULL, type));
			array_shift(tokens);
			linebreak(tokens);
		}
		else
			break;
	}
	return (create_ast_node(compounds, AST_COMPOUND_COMMAND));
}

t_ast_node *pipeline(t_array *tokens)
{
	t_token 		*token;
	t_array			*pipes;
	t_ast_node	*command_node;

	pipes = track_malloc(sizeof(t_array));
	array_init(pipes);
	while (true)
	{
		command_node = command(tokens);
		if (command_node == NULL)
			return NULL;
		array_push(pipes, command_node);
		token = array_peek(tokens);
		if (token->type != lexem_get_type("PIPE"))
			break;
		linebreak(tokens);
		array_shift(tokens);
	}
	return (create_ast_node(pipes, AST_PIPELINE));
}

t_ast_node	*command(t_array *tokens)
{
	t_token *token;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("OPEN_PARENTHESIS"))
	{
		array_shift(tokens);
		return (subshell(tokens));
	}
	return (simple_command(tokens));
}
  
t_ast_node *subshell(t_array *tokens)
{
	t_ast_node *node;
	t_token *token;

	node = command_list(tokens);
	if (node == NULL)
		return NULL;
	linebreak(tokens);
	token = array_peek(tokens);
	if (check_syntax_error(token->type != lexem_get_type("CLOSE_PARENTHESIS"), ERR_CLOSE_PARENTHESIS))
		return NULL;
	array_shift(tokens);
	node->type = AST_SUBSHELL;
	node->redirect_list = redirect_list(tokens);
	return node;
}

char *get_token_symbol(int type)
{
	if (type == lexem_get_type("WORD"))
		return "WORD";
	if (type == lexem_get_type("REDIRECTION"))
		return "REDIRECTION";
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
	return "UNKNOWN";
}

t_ast_node *simple_command(t_array *tokens)
{
	t_array	*argument_list;
	t_token *token;

	argument_list = track_malloc(sizeof(t_array));
	array_init(argument_list);
	while (true)
	{
		token = array_peek(tokens);
		if (lexem_is_redirection(token->type) || token->type == lexem_get_type("WORD"))
		{
			if (check_syntax_error(token->value == NULL, ERR_MISSING_FILENAME))
				return NULL;
			array_push(argument_list, token);
			array_shift(tokens);
			continue;
		}
		break;
	}
	if (check_syntax_error(argument_list->size == 0, ft_strjoin(ERR_WORD_OR_REDIR": ", get_token_symbol(token->type))))
		return NULL;
	return create_ast_node(argument_list, AST_SIMPLE_COMMAND);
}












t_array *redirect_list(t_array *tokens)
{
	t_token *token;
	t_array *redirections;

	redirections = track_malloc(sizeof(t_array));
	array_init(redirections);
	while (true)
	{
		token = array_peek(tokens);
		if (lexem_is_redirection(token->type) == false)
			break;
		if (token->value == NULL)
		{
			syntax_error("Missing filename");
			return NULL;
		}
		array_push(redirections, token);
		array_shift(tokens);
	}
	return (redirections);
}

t_token *io_redirect(t_array *tokens)
{
	t_token *token;

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

bool linebreak(t_array *tokens)
{
	t_token *token;

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

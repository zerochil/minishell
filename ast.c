#include "ast.h"
#include "libft/containers/array/array.h"
#include "libft/memory_management/memory_management.h"
#include "tokenizer.h"

char *syntax_error(char *message)
{
	static char *error_message;
	if (*message != '\0')
		error_message = message;
	return (error_message);
}

t_ast_node *create_ast_node(t_array *array, t_ast_type type)
{
	t_ast_node *node;

	node = track_malloc(sizeof(t_ast_node));
	node->type = type;
	node->children = array;
	node->redirect_list = NULL;
	node->error_message = NULL;
	return node;
}

static void	print_token(void *token_ptr)
{
	t_array	*lexems;
	t_token	*token;
	char	*id;
	lexems = lexems_get_instance();
	token = token_ptr;
	if (token->type == 0)
		id = "WORD";
	else if(token->type == -1)
		id = "EOF";
	else
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	printf("[%s, %s]", id, token->filename);
	fflush(NULL);
}





t_array *generate_ast(t_array *tokens)
{
	t_array *ast_list;
	t_ast_node *node;
	t_token *token;
	int		newline_type;	

	newline_type = lexem_get_type("NEWLINE");
	array_init(ast_list);
	while(true)
	{
		node = complete_command(tokens);
		token = array_peek(tokens);
		if (token->type != -1 && token->type != newline_type)
		{
			node->error_message = syntax_error("Unexpected token");
			while (token->type != -1 && token->type != newline_type)
			{
				array_shift(tokens);
				token = array_peek(tokens);
			}
		}
		if (token->type == -1)
			break;
	}
	array_push(ast_list, node);
	return ast_list;
}


t_ast_node *complete_command(t_array *tokens)
{
	t_ast_node	*command_list_node;
	t_ast_node	*node;
	t_array		*ast_list;

	node = create_ast_node(NULL, AST_COMPLETE_COMMAND);
	command_list_node = command_list(tokens);
	if (command_list_node == NULL)
	{
		node->error_message = syntax_error("WHY YOU??");
		return node;
	}
	return command_list_node;
}

t_ast_node *command_list(t_array *tokens)
{
	t_array	*commands;
	t_ast_node	*command;

	commands = track_malloc(sizeof(t_array));
	array_init(commands);
	linebreak(tokens);
	while(true)
	{
		command = compound_command(tokens);
		if (command == NULL)
			return NULL;
		array_push(commands, command);
		/*if (linebreak(tokens) == false)*/
		/*	break;*/
	}
	return (create_ast_node(commands, AST_COMMAND_LIST));
}

t_ast_node *compound_command(t_array *tokens)
{
	t_token *token;
	t_ast_node	*pipes;
	t_array *compounds;	

	compounds = track_malloc(sizeof(t_array));
	array_init(compounds);
	while(true)
	{
		pipes = pipeline(tokens);
		if (pipes == NULL)
			return (NULL);
		array_push(compounds, pipes);
		linebreak(tokens);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("LOGIC_AND"))
		{
			array_push(compounds, create_ast_node(NULL, AST_BINARY_AND));
			array_shift(tokens);
		}
		else if (token->type == lexem_get_type("LOGIC_OR"))
		{
			array_push(compounds, create_ast_node(NULL, AST_BINARY_OR));
			array_shift(tokens);
		}
		else
			break;
	}
	if (compounds->size <= 0)
	{
		report_error("");
		return NULL;
	}
	return (create_ast_node(compounds, AST_COMPOUND_COMMAND));
}

t_ast_node *pipeline(t_array *tokens)
{
	t_token 		*token;
	t_array			*pipes;
	t_ast_node	*command_node;
	int state;

	state = 1;
	pipes = track_malloc(sizeof(t_array));
	array_init(pipes);
	while (true)
	{
		command_node = command(tokens);
		array_do(tokens, print_token);
		printf("\n");
		if (command_node == NULL && state == 1)
		{
			// not found
			return NULL;
		}
		if (command_node == NULL && state == 2)
		{
			// unfinished pipeline
			return NULL;
		}
		array_push(pipes, command_node);
		token = array_peek(tokens);
		if (token->type != lexem_get_type("PIPE"))
			break;
		linebreak(tokens);
		state = 2;
		array_shift(tokens);
	}
	if (pipes->size == 0)
		return NULL;
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
	linebreak(tokens);
	if (node == NULL)
		return NULL;
	token = array_peek(tokens);
	if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
	{
		array_shift(tokens);
		node->type = AST_SUBSHELL;
		node->redirect_list = redirect_list(tokens);
	}
	else
	{
		report_error("");
		return NULL;
	}
	return node;
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
		if (lexem_is_redirection(token->type))
		{
			if (token->filename == NULL)
			{
				syntax_error("Unexpected token");
				return NULL;
			}
			array_push(argument_list, token);
			array_shift(tokens);
			continue;
		}
		if (token && token->type == lexem_get_type("WORD"))
		{
			array_push(argument_list, token);
			array_shift(tokens);
			continue;
		}
		break;
	}
	if (argument_list->size <= 0)
	{
		syntax_error("Unexpected token");
		return NULL;
	}
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
		if (token->type != lexem_get_type("REDIRECTION"))
			break;
		if (token->filename == NULL)
		{
			syntax_error("Missing filename");
			return NULL;
		}
		array_push(redirections, token);
	}
	return (redirections);
}

t_token *io_redirect(t_array *tokens)
{
	t_token *token;

	token = array_peek(tokens);
	if (lexem_is_redirection(token->type))
	{
		if (token->filename == NULL)
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

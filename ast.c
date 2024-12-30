#include "ast.h"

bool check_error(void)
{
	return false;
}

t_array *command_list(t_array *tokens)
{
	t_array	*commands;
	commands = track_malloc(sizeof(t_array));
	array_init(commands);

	linebreak(tokens);
	while(true)
	{
		array_push(commands, compound_command(tokens));
		if (linebreak(tokens) == false)
			break;
	}
	return (commands);
}

t_compound_node *create_compound_command(t_ast_type type, t_array *pipeline)
{
	t_compound_node *compound_command;
	compound_command = track_malloc(sizeof(t_compound_node));
	compound_command->type = type;
	compound_command->pipeline = pipeline;
	return compound_command;
}

t_array *compound_command(t_array *tokens)
{
	t_token *token;
	t_array	*pipes;
	t_array *compounds;

	compounds = track_malloc(sizeof(t_array));
	array_init(compounds);
	while(true)
	{
		pipes = pipeline(tokens);
		if (pipes->size <= 0)
			break;
		array_push(compounds, create_compound_command(AST_PIPELINE, pipes));
		linebreak(tokens);
		token = array_peek(tokens);
		if (token->type == lexem_get_type("LOGIC_AND"))
		{
			array_push(compounds, create_compound_command(AST_BINARY_AND, NULL));
			array_shift(tokens);
		}
		else if (token->type == lexem_get_type("LOGIC_OR"))
		{
			array_push(compounds, create_compound_command(AST_BINARY_OR, NULL));
			array_shift(tokens);
		}
	}
	if (compounds->size <= 0)
		report_error("");
	return (compounds);
}

t_array *pipeline(t_array *tokens)
{
	t_token 		*token;
	t_array			*pipes;
	t_node_command	*command_node;

	pipes = track_malloc(sizeof(t_array));
	array_init(pipes);
	while (true)
	{
		command_node = command(tokens);
		if (check_error())
			return (NULL);
		if (command_node == NULL)
			break;
		array_push(pipes, command_node);
		linebreak(tokens);
		token = array_peek(tokens);
		if (token->type != lexem_get_type("PIPE"))
			return (pipes);
	}
	return (pipes);
}

t_node_command	*command(t_array *tokens)
{
	t_node_command	*command_node;
	
	command_node = track_malloc(sizeof(t_node_command));
	command_node->subshell = subshell(tokens);
	if (check_error())
		return NULL;
	if (command_node->subshell.list != NULL)
	{
		command_node->type = AST_SUBSHELL;
		return command_node;
	}
	command_node->simple_command = simple_command(tokens);
	if (command_node->simple_command.size > 0)
	{
		command_node->type = AST_SIMPLE_COMMAND;
		return command_node;
	}
	report_error("");
	return NULL;
}
  
t_subshell subshell(t_array *tokens)
{
	t_token *token;
	t_subshell subshell_node;

	subshell_node.list = NULL;
	token = array_peek(tokens);
	if (token->type != lexem_get_type("OPEN_PARENTHESIS"))
		return subshell_node;
	array_shift(tokens);
	subshell_node.list = command_list(tokens);
	if (check_error())
		return subshell_node;
	token = array_peek(tokens);
	if (token->type == lexem_get_type("CLOSE_PARENTHESIS"))
	{
		array_shift(tokens);
		subshell_node.redirection_list = redirect_list(tokens);
	}
	else
		report_error("");
	return subshell_node;
}

t_array simple_command(t_array *tokens)
{
	t_token *token;
	t_array	argument_list;

	array_init(&argument_list);
	while (true)
	{
		token = io_redirect(tokens);
		if (token)
		{
			array_push(&argument_list, token);
			continue;
		}
		token = array_peek(tokens);
		if (token && token->type == lexem_get_type("WORD"))
		{
			array_push(&argument_list, token);
			array_shift(tokens);
			continue;
		}
		break;
	}
	return argument_list;
}

t_array redirect_list(t_array *tokens)
{
	t_token *token;
	t_array redirections;

	array_init(&redirections);
	while (true)
	{
		token = io_redirect(tokens);
		if (token == NULL)
			break;
		array_push(&redirections, token);
		array_shift(tokens);
	}
	return (redirections);
}

t_token *io_redirect(t_array *tokens)
{
	t_token *token;

	token = array_peek(tokens);
	if (lexem_is_redirection(token->type))
		return (array_shift(tokens));
	return (NULL);
}

bool linebreak(t_array *tokens)
{
	t_token *token;

	token = array_peek(tokens);
	if (token->type == lexem_get_type("NEWLINE"))
	{
		array_shift(tokens);
		linebreak(tokens);
		return (true);
	}
	return (false);
}

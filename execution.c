#include "execution.h"
#include "builtins.h"
#include "env.h"

void execution(t_context *context)
{
	t_ast_node *ast_root;

	while (true)
	{
		ast_root = array_shift(context->ast_root_list);
		if (ast_root == NULL)
			break;
		if (ast_root->children == NULL)
		{
			report_error(ast_root->error_message);
			continue;
		}
		execute_complete_command(ast_root);
		// TODO: set $?
	}
}

int execute_complete_command(t_ast_node *node)
{
	t_ast_node *command_list_node;

	command_list_node = array_shift(node->children);
	if (command_list_node == NULL)
		return (-1);
	return (execute_command_list(command_list_node));
}

int execute_command_list(t_ast_node *node)
{
	t_ast_node *compound_command_node;

	compound_command_node = array_shift(node->children);
	if (compound_command_node == NULL)
		return (-1);
	return(execute_compound_command(compound_command_node));
}

int execute_compound_command(t_ast_node *node)
{
	int last_exit_status;
	t_ast_node *command_node;

	last_exit_status = 0;
	while (true)
	{
		command_node = array_shift(node->children);
		if (command_node == NULL)
			break;
		if (command_node->type == AST_PIPELINE)
			last_exit_status = execute_pipeline(command_node);
		else if (command_node->type == AST_BINARY_AND)
		{
			if (last_exit_status != 0)
				break;
		}
		else if (command_node->type == AST_BINARY_OR)
		{
			if (last_exit_status == 0)
				break;
		}
		else
			error("execute_compound_command: error");
	}
	return (last_exit_status);
}


void fork_and_execute(t_array *commands, int index, t_pipeline_context *pipeline_context)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		error(NULL);
	}
	else if (pid > 0)
		pipeline_context->last_command_pid = pid;
	else if (pid == 0)
	{
		if (pipeline_context->read_end != STDIN_FILENO)
		{
			dup2(pipeline_context->read_end, STDIN_FILENO);
			close(pipeline_context->read_end);
		}
		if (pipeline_context->write_end != STDOUT_FILENO)
		{
			dup2(pipeline_context->write_end, STDOUT_FILENO);
			close(pipeline_context->write_end);
		}
		exit(execute_command(array_get(commands, index)));
	}
}

int execute_pipeline(t_ast_node *node)
{
	t_pipeline_context pipeline_context;
	t_array *commands;
	int index;
	int last_command_status;

	commands = node->children;
	if (commands->size == 1)
	{
		// TODO: if is not built-in, you should fork;
		return (execute_command(array_get(commands, 0)));
	}
	index = 0;
	while (index < commands->size)
	{
		//TODO: check if command is found first, otherwise error, index++;
		if (index < commands->size - 1)
			if (pipe((int *)&pipeline_context.pipe) == -1)
			{
				perror("minishell");
				error(NULL);
			}
		pipeline_context.write_end = pipeline_context.pipe.write_end;
		if (index == 0)
			pipeline_context.read_end = STDIN_FILENO;
		if (index == commands->size - 1)
			pipeline_context.write_end = STDOUT_FILENO;
		fork_and_execute(commands, index, &pipeline_context);
		if (pipeline_context.read_end != STDIN_FILENO)
			close(pipeline_context.read_end);
		if (pipeline_context.write_end != STDOUT_FILENO)
			close(pipeline_context.write_end);
		pipeline_context.read_end = pipeline_context.pipe.read_end;
		index++;
	}
	waitpid(pipeline_context.last_command_pid, &last_command_status, 0);
	while (wait(NULL) > 0)
		;
	return (last_command_status);
}

int execute_command(t_ast_node *node)
{
	if (node->type == AST_SUBSHELL)
		return (execute_subshell(node));
	else if (node->type == AST_SIMPLE_COMMAND)
		return (execute_simple_command(node));
	else
		error("execute_command_list: error");
}

int execute_subshell(t_ast_node *node)
{
	// (cat; io) < file;
	// hanndle redirection
	// and dup redirection
	// then call execute_command_list
	return (execute_command_list(node));
}

int execute_simple_command(t_ast_node *node)
{
	t_command_context command_context;
	int fd_out;
	int fd_in;

	fd_out = -1;
	fd_in = -1;
	if( handle_redirection(node->redirect_list, &fd_out, &fd_in) == -1)
		return (-1);
	if (fd_out == -1)
		fd_out = STDOUT_FILENO;
	if (fd_in == -1)
		fd_in = STDIN_FILENO;
	command_context.args = array_dup(node->children); // TODO: this isn't an array of strings;
	command_context.envp = env_get_array();
	command_context.fd_in = fd_in;
	command_context.fd_out = fd_out;
	// TODO: close fds for builtin or if execve fails
	if (is_builtin(command_context.args[0]))
		return (execute_builtin(command_context.args, command_context.fd_out));
	execve(command_context.args[0], command_context.args, command_context.envp);
	perror("minishell");
	return (127);
}


int handle_redirection(t_array *redirection_list, int *fd_out, int *fd_in)
{
	int open_error;
	t_token *token;
	t_array *token_list;

	while (true)
	{
		token_list = array_shift(redirection_list);
		if (token_list == NULL)
			break;
		if (token_list->size != 1)
			return (report_error("minishell: ambiguous redirect"), -1);
		token = array_shift(token_list);
		if (token->type == lexem_get_type("REDIRECTION_IN") || token->type == lexem_get_type("HERE_DOCUMENT"))
			open_error = open_file(token->value->data, O_RDONLY, fd_in);
		else if (token->type == lexem_get_type("REDIRECTION_TRUNC"))
			open_error = open_file(token->value->data, O_WRONLY | O_CREAT | O_TRUNC, fd_out);
		else if (token->type == lexem_get_type("REDIRECTION_APPEND"))
			open_error = open_file(token->value->data, O_WRONLY | O_CREAT | O_APPEND, fd_out);
		else
			error("handle_redirection: error");
		if (open_error == -1)
			return (-1);
	}
	return (0);
}

int open_file(char *filename, int flags, int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = open(filename, flags, 0644);
	if (*fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}

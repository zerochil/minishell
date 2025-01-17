#include "execution.h"

int execution(t_array *ast_root_list)
{
	t_ast_node *ast_root;
	int status;

	while (true)
	{
		ast_root = array_shift(ast_root_list);
		if (ast_root == NULL)
			break;
		if (ast_root->children == NULL)
		{
			report_error(ast_root->error_message);
			status = 2;
			continue;
		}
		status = execute_complete_command(ast_root);
		// TODO: set $?
	}
	return (status);
}

int execute_complete_command(t_ast_node *node)
{
	t_ast_node *compound_command_node;

	compound_command_node = array_shift(node->children);
	if (compound_command_node == NULL)
		return (-1);
	return (execute_compound_command(compound_command_node));
}

/*int execute_command_list(t_ast_node *node)*/
/*{*/
/*	t_ast_node *compound_command_node;*/
/**/
/*	compound_command_node = array_shift(node->children);*/
/*	if (compound_command_node == NULL)*/
/*		return (-1);*/
/*	return(execute_compound_command(compound_command_node));*/
/*}*/

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

char *get_command_name(t_ast_node *command_node)
{
	char *command_name;
	t_token *token;

	command_name = NULL;
	if (command_node->type == AST_SIMPLE_COMMAND)
	{
		token = array_get(command_node->children, 0);
		if (token == NULL)
			return (NULL);
		command_name = token->value;
	}
	else
		error("get_command_name: error");
	return (command_name);
}



int execute_single_command(t_ast_node *command_node)
{

	char *command_name;
	pid_t pid;
	int status;

	if (command_node->type == AST_SUBSHELL)
		return (execute_subshell(command_node));
	else if (command_node->type == AST_SIMPLE_COMMAND)
	{
		command_name = get_command_name(command_node);
		if (command_name == NULL || is_builtin(command_name))
			return (execute_simple_command(command_node));
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("minishell");
				error(NULL);
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					return (WEXITSTATUS(status));
				else if (WIFSIGNALED(status))
					return (WTERMSIG(status) + 128);
			}
			else if (pid == 0)
			{
				if (command_node->type == AST_SIMPLE_COMMAND)
				{
					return (execute_simple_command(command_node));
				}
			}
		}
	}
	else
		error("execute_single_command: error");
	return (-1);
}

void fork_and_execute(t_array *commands, size_t index, t_pipeline_context *pipeline_context)
{
	pid_t pid;
	int status;

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
			close(pipeline_context->write_end);;
		}
		if (index < commands->size - 1)
			close(pipeline_context->pipe.read);
		status = execute_command(array_get(commands, index));
		destroy_context();
		exit(status);
	}
}

int execute_pipeline(t_ast_node *node)
{
	t_pipeline_context pipeline_context;
	t_array *commands;
	size_t index;
	int last_command_status;

	commands = node->children;
	if (commands->size == 1)
		return (execute_single_command(array_get(commands, 0)));
	index = 0;
	while (index < commands->size)
	{
		if (index < commands->size - 1)
		{
			if (pipe((int *)&pipeline_context.pipe) == -1)
			{
				perror("minishell");
				error(NULL);
			}
		}
		pipeline_context.write_end = pipeline_context.pipe.write;
		if (index == 0)
			pipeline_context.read_end = STDIN_FILENO;
		if (index == commands->size - 1)
			pipeline_context.write_end = STDOUT_FILENO;
		fork_and_execute(commands, index, &pipeline_context);
		if (pipeline_context.read_end != STDIN_FILENO)
			close(pipeline_context.read_end);
		if (pipeline_context.write_end != STDOUT_FILENO)
			close(pipeline_context.write_end);
		pipeline_context.read_end = pipeline_context.pipe.read;
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
	return (-1);
}

int execute_subshell(t_ast_node *node)
{
	pid_t pid;
	int status;
	int fd_out;
	int fd_in;

	fd_out = -1;
	fd_in = -1;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		error(NULL);
	}
	else if (pid == 0)
	{
		if (handle_redirection(node->redirect_list, &fd_out, &fd_in) == -1)
		{
			destroy_context();
			exit(1);
		}
		if (fd_out != -1)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		if (fd_in != -1)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		status = execute_compound_command(array_get(node->children, 0));
		destroy_context();
		exit(status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (WTERMSIG(status) + 128);
}

char **build_arg_list(t_array *fields)
{
	t_field *field;
	char **args;
	size_t i;

	args = track_malloc(sizeof(char *) * (fields->size + 1));
	i = 0;
	while (i < fields->size)
	{
		field = array_get(fields, i);
		args[i] = field->value->data;
		i++;
	}
	args[i] = NULL;
	return (args);
}

char **get_arg_list(t_array	*tokens)
{
	t_token *token;
	t_array	fields;
	size_t	i;

	array_init(&fields);
	i = 0;
	while (i < tokens->size)
	{
		token = array_get(tokens, i);
		array_merge(&fields, token->fields);
		i++;
	}
	return (build_arg_list(&fields));
}

char *get_command_path(char *command_name)
{
	char *path;
	char *path_env;
	char **path_list;
	size_t i;
	char command_path[PATH_MAX];

	if (ft_strchr(command_name, '/') != NULL)
		return (command_name);
	path_env = env_get("PATH");
	if (path_env == NULL)
		return (command_name);
	path_list = ft_split(path_env, ':');
	resource_track(path_list, free_strings);
	i = 0;
	while (path_list[i])
	{
		command_path[0] = '\0';
		ft_strlcat(command_path, path_list[i], sizeof(command_path));
		ft_strlcat(command_path, "/", sizeof(command_path));
		ft_strlcat(command_path, command_name, sizeof(command_path));
		if (access(command_path, F_OK) == 0)
		{
			path = ft_strdup(command_path);
			resource_track(path, free);
			return (path);
		}
		i++;
	}
	return (NULL);
}

int execute_external(t_command_context *command_context)
{
	char *command_path;

	// TODO: if dup2 fails??
	dup2(command_context->fd_in, STDIN_FILENO);
	dup2(command_context->fd_out, STDOUT_FILENO);
	command_path = get_command_path(command_context->args[0]);
	if (command_path == NULL)
	{
		report_error("minishell: command not found");
		destroy_context();
		exit(127);
	}
	command_context->args[0] = command_path;
	execve(command_context->args[0], command_context->args, command_context->envp);
	perror("minishell: external");
	return (127);
}

int execute_builtin(char **args, int out_fd)
{
	builtin_t *builtins;
	int i;

	builtins = get_builtins_instance();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(builtins[i].name, args[0]) == 0)
			return (builtins[i].function(args, out_fd));
		i++;
	}
	return (-1);
}

int execute_simple_command(t_ast_node *node)
{
	t_command_context command_context;
	int fd_out;
	int fd_in;
	int status;

	fd_out = -1;
	fd_in = -1;
	if( handle_redirection(node->redirect_list, &fd_out, &fd_in) == -1)
		return (-1);
	if (fd_out == -1)
		fd_out = STDOUT_FILENO;
	if (fd_in == -1)
		fd_in = STDIN_FILENO;
	command_context.args = get_arg_list(node->children);
	command_context.envp = env_get_array();
	command_context.fd_in = fd_in;
	command_context.fd_out = fd_out;
	status = 0;
	if (command_context.args[0] != NULL)
	{
		if (is_builtin(command_context.args[0]))
			status = execute_builtin(command_context.args, command_context.fd_out);
		else
			execute_external(&command_context);
	}

	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	return (status);
}

int handle_redirection(t_array *redirection_list, int *fd_out, int *fd_in)
{
	int open_error;
	t_token *token;
	t_field *field;

	while (true)
	{
		token = array_shift(redirection_list);
		if (token == NULL)
			break;
		if (token->fields->size != 1)
			return (report_error("minishell: ambiguous redirect"), -1);
		field = array_shift(token->fields);
		if (token->type == lexem_get_type("REDIRECTION_IN") || token->type == lexem_get_type("HERE_DOCUMENT"))
			open_error = open_file(field->value->data, O_RDONLY, fd_in);
		else if (token->type == lexem_get_type("REDIRECTION_TRUNC"))
			open_error = open_file(field->value->data, O_WRONLY | O_CREAT | O_TRUNC, fd_out);
		else if (token->type == lexem_get_type("REDIRECTION_APPEND"))
			open_error = open_file(field->value->data, O_WRONLY | O_CREAT | O_APPEND, fd_out);
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

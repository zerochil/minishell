#include "execution.h"

void clean_exit(int status)
{
	destroy_context();
	exit(status);
}

typedef struct s_context
{
	t_array	*ast_root_list;
	t_array *pids;
} t_context;

t_context *get_context_instance()
{
	static t_context context;

	if (context.pids == NULL)
	{
		/*printf("jidosa");*/
		context.pids = track_malloc(sizeof(t_array));
		array_init(context.pids);
	}
	return (&context);
}


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







// ->> PID MANAGEMENT

int is_pid(void *elment_ptr, void *target_ptr)
{
	pid_t pid;
	pid_t target;

	pid = (pid_t)(intptr_t)elment_ptr;
	target = (pid_t)(intptr_t)target_ptr;
	return (pid == target);
}

void pid_remove(pid_t pid)
{
	// TODO: change this so it's array_do compatible??
	t_context *context;
	pid_t p;

	context = get_context_instance();
	p = array_index_of(context->pids, (void *)(intptr_t)pid, is_pid);
	if (p != -1)
		array_remove(context->pids, p);
}

void pid_push(pid_t pid)
{
	t_context *context;

	context = get_context_instance();
	array_push(context->pids, (void *)(intptr_t)pid);
}

// ->> END PID MANAGEMENT








// ->> PIPLINE

int should_not_fork(t_array *commands)
{
	t_ast_node *command_node;
	t_token *token;
	t_field	*field;
	char *command_name;

	if (commands->size != 1)
		return (0);
	command_node = array_get(commands, 0);
	if (command_node->type != AST_SIMPLE_COMMAND)
		return (0);
	token = array_get(command_node->children, 0);
	if (token == NULL)
		return (0);
	field = array_get(token->fields, 0);
	command_name = field->value->data;
	if (is_builtin(command_name) == 0)
		return (0);
	return (1);
}

t_stream *stream_init(int pipeline_size)
{
	t_stream *streamline;
	int i;

	streamline = track_malloc(sizeof(t_stream) * (pipeline_size + 1));
	i = 0;
	while (i < pipeline_size)
	{
		streamline[i].read = STDIN_FILENO;
		streamline[i].write = STDOUT_FILENO;
		i++;
	}
	return (streamline);
}

void create_pipe(t_stream *streamline, int index)
{
	t_stream p;

	if (pipe((int *)&p) == -1)
	{
		perror("minishell");
		error(NULL);
	}
	streamline[index].write = p.write;
	streamline[index + 1].read = p.read;
}

void stream_close(t_stream *stream)
{
	if (stream->read >= 0 && stream->read != STDIN_FILENO)
	{
		close(stream->read);
		stream->read = -1;
	}
	if (stream->write >= 0  && stream->write != STDOUT_FILENO)
	{
		close(stream->write);
		stream->write = -1;
	}
}

void stream_dup2stdio(t_stream *stream)
{
	if (stream->read != STDIN_FILENO)
		if (dup2(stream->read, STDIN_FILENO) == -1)
			error("dup2: error");
	if (stream->write != STDOUT_FILENO)
		if (dup2(stream->write, STDOUT_FILENO) == -1)
			error("dup2: error");
	stream_close(stream);
} 

pid_t fork_and_execute(t_array *commands, t_stream *streamline, size_t index, int *pid)
{
	//pid_t pid;
	int status;

	if (should_not_fork(commands))
		return (execute_command(array_get(commands, index)));
	*pid = fork();
	if (*pid == -1)
	{
		perror("minishell");
		error(NULL);
	}
	else if (*pid > 0)
		return (*pid);
	stream_dup2stdio(&streamline[index]);
	if (index != commands->size - 1)
		stream_close(&streamline[index + 1]);
	status = execute_command(array_get(commands, index));
	clean_exit(status);
	return (-1);
}

int execute_pipeline(t_ast_node *node)
{
	t_stream *streamline;
	t_array *commands;
	size_t index;
	int status;
	pid_t pid;

	commands = node->children;
	index = 0;
	streamline = stream_init(commands->size);
	pid = -1;
	while (index < commands->size)
	{

		handle_expansions(array_get(commands, index));
		if (index < commands->size - 1)
			create_pipe(streamline, index);
		status = fork_and_execute(commands, streamline, index, &pid);
		stream_close(&streamline[index]);
		// pid_push(pid);
		index++;
	}
	if (pid > 0)
		waitpid(pid, &status, 0);
	while (wait(NULL) > 0)
		;
	/*parameter_set("?", ft_itoa(WEXITSTATUS(status)));*/
	return (status);
}

// ->> END PIPELINE








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
	int status;
	t_stream stream;

	stream = (t_stream){.read = STDIN_FILENO, .write = STDOUT_FILENO};
	if (handle_redirection(node->redirect_list, &stream) == -1)
		return (-1);
	stream_dup2stdio(&stream);
	status = execute_compound_command(array_get(node->children, 0));
	clean_exit(WEXITSTATUS(status));
	return (-1);
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

char **get_arg_list(t_array *tokens)
{
        t_token *token;
        t_array fields;
        size_t  i;

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

char *build_command_path(char *path, char *command_name)
{
	char *command_path;
	int len;

	len = ft_strlen(path) + ft_strlen(command_name) + 2;
	command_path = track_malloc(len);
	command_path[0] = '\0';
	ft_strlcat(command_path, path, len);
	ft_strlcat(command_path, "/", len);
	ft_strlcat(command_path, command_name, len);
	return (command_path);
}

char *get_command_path(char *command_name)
{
	char *path;
	char *path_env;
	char **path_list;
	size_t i;
	char *command_path;

	if (ft_strchr(command_name, '/') != NULL)
		return (command_name);
	path_env = env_get("PATH");
	if (path_env == NULL)
		return (command_name);
	path_list = ft_split(path_env, ':');
	resource_track(path_list, free_strings);
	i = 0;
	while (path_list[i] != NULL)
	{
		path = path_list[i];
		command_path = build_command_path(path, command_name);
		if (access(command_path, F_OK) == 0)
			return (command_path);
		resource_free(command_path);
		i++;
	}
	return (NULL);
}

int execute_external(t_command_context *command_context)
{
	char *command_path;

	stream_dup2stdio(&command_context->stream);
	command_path = get_command_path(command_context->args[0]);
	if (command_path == NULL || access(command_path, F_OK) == -1)
	{
		report_error("minishell: command not found");
		return (127);
	}
	command_context->args[0] = command_path;
	execve(command_context->args[0], command_context->args, command_context->envp);
	perror("minishell: external");
	return (127); // TODO: check if this is correct
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
	int status;

	command_context.stream = (t_stream){.read = STDIN_FILENO, .write = STDOUT_FILENO};
	if(handle_redirection(node->redirect_list, &command_context.stream) == -1)
		return (1);
	command_context.args = get_arg_list(node->children);
	command_context.envp = env_get_array();
	status = 0;
	if (command_context.args[0] != NULL)
	{
		if (is_builtin(command_context.args[0]))
			status = execute_builtin(command_context.args, command_context.stream.write);
		else
			status = execute_external(&command_context);
	}
	stream_close(&command_context.stream);
	return (status);
}

int handle_redirection(t_array *redirection_list, t_stream *stream)
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
			open_error = open_file(field->value->data, O_RDONLY, &stream->read);
		else if (token->type == lexem_get_type("REDIRECTION_TRUNC"))
			open_error = open_file(field->value->data, O_WRONLY | O_CREAT | O_TRUNC, &stream->write);
		else if (token->type == lexem_get_type("REDIRECTION_APPEND"))
			open_error = open_file(field->value->data, O_WRONLY | O_CREAT | O_APPEND, &stream->write);
		else
			error("handle_redirection: error");
		if (open_error == -1)
			return (stream_close(stream), -1);
	}
	return (0);
}

int open_file(char *filename, int flags, int *fd)
{
	if (*fd != -1 && *fd != STDIN_FILENO && *fd != STDOUT_FILENO)
		close(*fd);
	*fd = open(filename, flags, 0644);
	if (*fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}

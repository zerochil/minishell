#ifndef EXECUTION_H
# define EXECUTION_H

#include "ast.h"
#include "lexer.h"
#include "tokenizer.h"
#include <sys/wait.h>

typedef struct s_pipe
{
	int read_end;
	int write_end;
} t_pipe;

typedef struct s_pipeline_context
{
	int last_command_pid;
	t_pipe pipe;
	int write_end;
	int read_end;
} t_pipeline_context;

typedef struct s_command_context
{
	char	**args;
	char	**envp;
	int		fd_out;
	int		fd_in;
} t_command_context;

typedef struct s_context
{
	t_array	*ast_root_list;

} t_context;

int execution(t_context *context);
int execute_ast_node(t_ast_node *node);
int execute_complete_command(t_ast_node *node);
int execute_compound_command(t_ast_node *node);
int execute_command_list(t_ast_node *node);
int execute_command(t_ast_node *node);
int execute_pipeline(t_ast_node *node);
int execute_subshell(t_ast_node *node);
int execute_simple_command(t_ast_node *node);
int handle_redirection(t_array *token_list, int *fd_out, int *fd_in);
int open_file(char *filename, int flags, int *fd);

#endif

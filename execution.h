#ifndef EXECUTION_H
# define EXECUTION_H

#include <sys/wait.h>
#include <limits.h>

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>


#include "ast.h"
#include "lexer.h"
#include "tokenizer.h"
#include "builtins.h"
#include "env.h"
#include "utils.h"


#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

typedef struct s_stream
{
	int read;
	int write;
} t_stream;

typedef struct s_command_context
{
	char		**args;
	char		**envp;
	t_stream	stream;
} t_command_context;

int execution(t_array *ast_root_list);
int execute_ast_node(t_ast_node *node);
int execute_complete_command(t_ast_node *node);
int execute_compound_command(t_ast_node *node);
int execute_command_list(t_ast_node *node);
int execute_command(t_ast_node *node);
int execute_pipeline(t_ast_node *node);
int execute_subshell(t_ast_node *node);
int execute_simple_command(t_ast_node *node);

int handle_redirection(t_array *redirection_list, t_stream *stream);
int open_file(char *filename, int flags, int *fd);

#endif

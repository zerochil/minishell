/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:35:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/20 16:16:39 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "ast.h"
# include "stream.h"
# include "execution_pipeline.h"


typedef struct s_command_context
{
	char		**args;
	char		**envp;
	t_stream	stream;
}				t_command_context;

int				execution(t_array *ast_root_list);
int				execute_ast_node(t_ast_node *node);
int				execute_complete_command(t_ast_node *node);
int				execute_compound_command(t_ast_node *node);
int				execute_command_list(t_ast_node *node);
int				execute_command(t_ast_node *node);
int				execute_pipeline(t_ast_node *node);
int				execute_subshell(t_ast_node *node);
int				execute_simple_command(t_ast_node *node);

char			**build_arg_list(t_array *fields);
char			**get_arg_list(t_array *tokens);
char			*build_command_path(char *path, char *command_name);
char			*get_command_path(char *command_name);

int				open_redirection_files(t_array *redirection_list,
					t_stream *stream);
int				open_file(char *filename, int flags, int *fd);

void			clean_exit(int status);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:36:18 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 22:10:04 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "libft/libft.h"
# include "tokenizer.h"
#include "lexer.h"

typedef enum e_ast_type
{
	AST_BINARY,
	AST_BINARY_OR,
	AST_BINARY_AND,
	AST_PIPELINE,
	AST_COMMAND,
	AST_SUBSHELL,
	AST_SIMPLE_COMMAND,
}							t_ast_type;

typedef struct s_ast		t_ast;

typedef struct s_compound_node
{
	t_ast_type			type;
	t_array				*pipeline;
}	t_compound_node;

typedef struct s_subshell
{
	t_array					*list;
	t_array					redirection_list;
}							t_subshell;

typedef struct s_node_command
{
	t_ast_type				type;
	union
	{
		t_subshell			subshell;
		t_array				simple_command;
	};
}							t_node_command;


t_compound_node *create_compound_command(t_ast_type type, t_array *pipeline);
t_array *command_list(t_array *tokens);
t_array *compound_command(t_array *tokens);
t_array *pipeline(t_array *tokens);
t_node_command	*command(t_array *tokens);
t_subshell subshell(t_array *tokens);
t_array simple_command(t_array *tokens);
t_array redirect_list(t_array *tokens);
t_token *io_redirect(t_array *tokens);
bool linebreak(t_array *tokens);

bool check_error();

#endif

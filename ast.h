/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:36:18 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 16:36:44 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "libft/libft.h"

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

typedef struct s_node_binary
{
	t_ast_type				type;
	t_ast					*left;
	t_ast					*right;
}							t_node_binary;

typedef struct s_node_pipeline
{
	t_array					*commands;
}							t_node_pipeline;

typedef struct s_subshell
{
	t_ast					*list;
	t_array					*redirection_list;
}							t_subshell;

typedef struct s_simple_command
{
	t_array					*tokens;
}							t_simple_command;

typedef struct s_node_command
{
	t_ast_type				type;
	union
	{
		t_subshell			*subshell;
		t_simple_command	*simple_command;
	};
}							t_node_command;

struct						s_ast
{
	t_ast_type				type;
	union
	{
		t_node_binary		*binary;
		t_node_pipeline		*pipeline;
		t_node_command		*command;
	};
};

#endif

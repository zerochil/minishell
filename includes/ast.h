/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:36:18 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/22 10:23:49 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "libft.h"
# include "tokenizer.h"
# include "lexer.h"

# define ERR_WORD_OR_REDIR "syntax error near unexpected token: expected word or redirection"
# define ERR_MISSING_FILENAME "syntax error near unexpected token: missing filename"
# define ERR_EMPTY_COMMAND "syntax error near unexpected token: empty command"
# define ERR_UNFINISHED_PIPE "syntax error near unexpected token: unfinished pipe"
# define ERR_CLOSE_PARENTHESIS "syntax error near unexpected token: missing close parenthesis"

typedef enum e_ast_type
{
	AST_COMPOUND_COMMAND,
	AST_COMMAND,
	AST_PIPELINE,
	AST_SUBSHELL,
	AST_SIMPLE_COMMAND,
	AST_BINARY_OR,
	AST_BINARY_AND,
	AST_INVALID_COMMAND,
}							t_ast_type;

typedef struct 				s_ast_node
{
	t_ast_type				type;
	t_array					*children;
	t_array					*redirect_list;
	char 					*error_message;
}							t_ast_node;

/*t_array *generate_ast(t_array *tokens);*/
/*t_ast_node *complete_command(t_array *tokens);*/
t_array	*complete_command(t_array *tokens);
t_array	*command_list(t_array *tokens);
t_ast_node *compound_command(t_array *tokens);
t_ast_node *pipeline(t_array *tokens);
t_ast_node *command(t_array *tokens);
t_ast_node *subshell(t_array *tokens);
t_ast_node *simple_command(t_array *tokens);

t_array *redirect_list(t_array *tokens);
t_token *io_redirect(t_array *tokens);
bool	skip_linebreak(t_array *tokens);

bool check_error();

#endif

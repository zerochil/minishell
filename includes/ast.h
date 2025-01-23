/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:36:18 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 08:45:55 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <libft.h>
# include <tokenizer.h>
# include <lexer.h>

# define ERR_UNEXPECTED "syntax error near unexpected token"
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
t_array	*generate_ast(t_array *tokens);
t_ast_node *compound_command(t_array *tokens);
t_ast_node *pipeline(t_array *tokens);
t_ast_node *command(t_array *tokens);
t_ast_node *subshell(t_array *tokens);
t_ast_node *simple_command(t_array *tokens);

char	*syntax_error(char *message);
bool	check_syntax_error(bool condition, char *message);


bool	skip_linebreak(t_array *tokens);
int	is_logical_and_or(t_array *tokens);
t_ast_node	*create_ast_node(t_array *children, t_ast_type type);
t_array *consume_until(t_array *tokens, bool is_target(int), bool to_skip(int), bool is_error(t_token *));

bool is_filename_missing(t_token *token);
bool is_not_valid_redirection(t_token *token);


#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:37:31 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "ast.h"
# include "parameter_expansion.h"
# include "field.h"
# include "pathname_expansion.h"
# include "context.h"

enum
{
	NO_EXPANSIONS = 0,
	PARAMETER_EXPANSION = 1,
	FIELD_SPLITTING = 2,
	PATHNAME_EXPANSION = 4,
	QUOTE_REMOVAL = 8,
	ALL_EXPANSIONS = 15,
};

bool	is_export_command(t_token *token);
bool	is_assingment_word(t_token *token);

void	single_field_quote_removal(void *field_ptr);
void	quote_removal(void *token_ptr);

void	handle_expansions(t_ast_node *command);
void	expansion(t_array *tokens);

#endif

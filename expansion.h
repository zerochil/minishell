/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/18 10:10:28 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "here_document.h"
# include "ast.h"
# include "field.h"
# include <dirent.h>

enum
{
	ORIGINAL = 0,		// 000
	EXPANDED = 1, 		// 001                
	SINGLE_QUOTED = 2,  // 010
	DOUBLE_QUOTED = 4,  // 100
};

enum
{
	NO_EXPANSIONS = 0,
	PARAMETER_EXPANSION = 1,
	FIELD_SPLITTING = 2,
	PATHNAME_EXPANSION = 4,
	QUOTE_REMOVAL = 8,
	ALL_EXPANSIONS = 15,
};

void	handle_expansions(t_ast_node *command);
void	expansion(t_array *tokens);

bool	expand_field_parameter(t_field *field, int single_quoted_flag);
bool	remove_quotes_from_field(t_field *field);

void	field_set(t_field *field, char *value);

#endif

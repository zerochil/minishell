/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/15 17:10:18 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
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

void	parameter_expansion(void *token_ptr);
void	field_splitting(void *token_ptr);
void	pathname_expansion(void *token_ptr);
void	quote_removal(void *token_ptr);

bool	expand_field_parameter(t_field *field, int single_quoted_flag);
bool	remove_quotes_from_field(t_field *field);


void	field_set(t_field *field, char *value);

#endif

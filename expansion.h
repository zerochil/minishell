/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/14 18:00:20 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include "ast.h"
# include <dirent.h>

typedef struct s_field
{
	t_string	*value;
	t_string	*mask;
}	t_field;

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

bool	expand_parameter(t_field *field);
#endif

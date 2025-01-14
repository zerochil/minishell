/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/13 22:56:42 by inajah           ###   ########.fr       */
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


void	parameter_expansion(void *token_ptr);
void	field_splitting(void *token_ptr);
void	pathname_expansion(void *token_ptr);
void	quote_removal(void *token_ptr);

void	expand_parameter(t_field *field);
#endif

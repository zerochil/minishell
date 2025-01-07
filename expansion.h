/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:13:33 by inajah            #+#    #+#             */
/*   Updated: 2025/01/07 18:19:33 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include "ast.h"
# include <dirent.h>

typedef struct s_field
{
	int		type;
	t_array *tokens;
}	t_field;

void	expansion(t_ast_node *node);
void	pathname_expansion(t_ast_node *node);
void	quote_removal(t_ast_node *node);

#endif

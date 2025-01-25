/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:36:47 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 11:42:51 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMETER_EXPANSION_H
# define PARAMETER_EXPANSION_H

# include "env.h"
# include "field.h"
# include "libft.h"
# include "token.h"

char	*parameter_get(char *parameter_name);
char	*get_parameter_name(t_string *string, size_t dolar_position);
int		expand_parameter(t_string *string, char *parameter_name);
void	parameter_expansion(void *token_ptr);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 09:28:59 by inajah            #+#    #+#             */
/*   Updated: 2025/01/25 09:18:44 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

#include "libft.h"
#include "field.h"
#include "tokenizer.h"
#include "ast.h"


void	print_vars(void *str_ptr);
void	print_field(void *field_ptr);
void	print_token(void *token_ptr);
void	print(void *node_ptr);

#endif

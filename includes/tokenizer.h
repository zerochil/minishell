/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:33 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/22 11:44:50 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <field.h>
# include <here_document.h>
# include <lexer.h>
# include <libft.h>
# include <token.h>
# include <context.h>

# define WHITE_SPACE " \t"
# define TOKENIZER_ERROR_MSG "tokenizer: unbalanced quotes!"
# define TOKENIZER_ERROR_EXIT_STATUS 2

t_array	*tokenize(t_string *input);
t_token	*token_init(int type, char *value);
int		is_quoted(t_string *input);
t_token	*tokenize_next(t_string *input);

#endif

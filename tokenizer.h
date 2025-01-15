/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:33 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/14 20:55:01 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "lexer.h"
# include "field.h"
# include "libft/libft.h"

# define META_CHARACTERS "|&()<>\n \t"
# define WHITE_SPACE " \t"

typedef struct s_token
{
	int			type;
	char		*value;
	bool		should_field_split;
	t_array		*fields; 
}				t_token;

t_array	*tokenize(t_string *input);
t_token	*token_init(int type, char *value);
int	is_quoted(t_string *input);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:33 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/13 20:15:09 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "lexer.h"
# include "libft/libft.h"

# define META_CHARACTERS "|&()<>\n \t"
# define WHITE_SPACE " \t"

typedef struct s_token
{
	int			type;
	char		*value;
	t_array		*fields; //TODO: make this work! 
}				t_token;

t_array	*tokenize(t_string *input);
t_token	*token_init(int type, char *value);
int	is_quoted(t_string *input);

#endif

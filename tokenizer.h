/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:29:33 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 19:54:41 by rrochd           ###   ########.fr       */
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
	union
	{
		char	*word;
		char	*filename;
	};
}				t_token;

t_array			*tokenize(t_string *input);
int	is_quoted(t_string *input);

#endif

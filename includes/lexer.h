/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:11:53 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/22 10:25:04 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <libft.h>

# define LEXEM_EOF -1

typedef struct s_lexem
{
	int		type;
	char	*symbol;
	char	*identifier;
	size_t	symbol_length;
	size_t	identifier_length;
}			t_lexem;

int			lexem_get_type(char *identifier);
t_array		*lexems_get_instance(void);

int			lexem_match_symbol(void *lexem_ptr, void *input_ptr);
int			lexem_match_identifier(void *lexem_ptr, void *identifier);
int			lexem_match_word_break(void *lexem_ptr, void *input_ptr);

bool	lexem_is_redirection(int type);
bool lexem_is_word(int type);
bool	lexem_is_cmd_word(int type);

#endif

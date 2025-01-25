/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/25 11:50:16 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H

# include <errno.h>
# include <field.h>
# include <libft.h>
# include <parameter_expansion.h>
# include <readline/readline.h>
# include <signals.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <termios.h>
# include <token.h>
# include <utils.h>

# define WARN_HEREDOC_DELIM \
"minishell: warning here_document delimited by end-of-file"

char	*random_filename(void);
size_t	get_second_line(char *input, size_t start);
void	expand_parameter_here_doc(t_string *content);
bool	is_delimiter(char *line, char *delimiter);

char	*create_here_document(t_string *input, t_field *delimiter);

#endif

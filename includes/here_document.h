/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 11:12:35 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H

# include "libft.h"
# include "field.h"
# include "signals.h"
# include "utils.h"

# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <termios.h>

# define WARN_HEREDOC_DELIM \
	"minishell: warning here_document delimited by end-of-file"

char	*random_filename(void);
size_t	get_second_line(char *input, size_t start);
void	expand_parameter_here_doc(t_string *content);
bool	is_delimiter(char *line, char *delimiter);

char	*create_here_document(t_string *input, t_field *delimiter);

#endif

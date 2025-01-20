/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/20 18:28:17 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H

# include <signal.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include "libft/libft.h"
# include "field.h"
# include <readline/history.h>
# include <readline/readline.h>

bool	remove_quotes_from_field(t_field *field);
char *create_here_document(t_string *input, t_field *delimiter);

#endif

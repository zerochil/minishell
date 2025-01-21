/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/21 09:37:52 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H

# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <readline/readline.h>

# include "libft/libft.h"
# include "field.h"
# include "signals.h"

bool	remove_quotes_from_field(t_field *field);
char *create_here_document(t_string *input, t_field *delimiter);

#endif

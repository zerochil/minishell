/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/19 14:20:36 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H


# include "libft/libft.h"
# include "field.h"
# include <readline/history.h>
# include <readline/readline.h>

bool	remove_quotes_from_field(t_field *field);
char *get_here_document_content(t_string *input, t_field *delimiter);
char *here_document_to_temp_file(char *here_document_content);

#endif

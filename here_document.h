/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:06:13 by inajah            #+#    #+#             */
/*   Updated: 2025/01/10 09:01:41 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOCUMENT_H
# define HERE_DOCUMENT_H


# include "libft/libft.h"
# include "tokenizer.h"
# include "expansion.h"
# include <readline/history.h>
# include <readline/readline.h>

void	handle_heredoc(void *token_ptr);

#endif

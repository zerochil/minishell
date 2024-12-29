/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 08:59:51 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 18:38:29 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "../base/base.h"
# include "../containers/containers.h"
# include "../memory_management/memory_management.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

# ifndef FOPEN_MAX
#  define FOPEN_MAX 1024
# endif

char	*get_next_line(int fd);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _containers_dependencies.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:07:20 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/21 19:02:31 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONTAINERS_DEPENDENCIES_H
# define _CONTAINERS_DEPENDENCIES_H

# include "../base/base.h"
# include "../memory_management/memory_management.h"

# define INITIAL_CAPACITY 16

void		*container_grow(void *old_data, size_t old_cap, size_t new_cap,
				size_t elem_size);

#endif

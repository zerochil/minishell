/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 08:59:51 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/11 21:02:59 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGEMENT_H
# define MEMORY_MANAGEMENT_H

# include "../base/base.h"
# include "fcntl.h"
# include "unistd.h"

typedef struct t_resource
{
	void				*resource;
	void				(*cleanup)(void *);
	struct t_resource	*next;
}						t_resource;

typedef struct t_manager
{
	t_resource			*resources;
	struct t_manager	*next;
}						t_manager;

void					error(char *message);
void          report_error(char *message);

void					free_resource(void *resource);
void					free_all_resources(void);
void					free_strings(void *strings);
void					close_fd(void *fd_ptr);

t_manager				**get_resource_managers(void);
t_manager				**ensure_default_manager(void);

void					add_scope(void);
void					end_scope(void);
void					track_resource(void *resource, void (*cleanup)(void *));

void					*safe_malloc(size_t size);
void					*track_malloc(size_t size);
int						track_open(char *pathname, int flags, mode_t mode,
							char *error_message);

void					*int_to_void(int n);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 08:59:51 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 14:50:46 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGEMENT_H
# define MEMORY_MANAGEMENT_H

# include "../base/base.h"
# include <fcntl.h>
# include <unistd.h>

typedef struct s_rm_node	t_rm_node;

struct s_manager
{
	char		*name;
	t_rm_node	*resources;
};

struct s_resource
{
	void		*pointer;
	void		(*cleanup)(void *);
};

struct s_rm_node
{
	union
	{
		struct s_resource	resource;
		struct s_manager	manager;
	};
	t_rm_node			*next;
};

t_rm_node	*rm_node_new_manager(char *name);
void		rm_node_add_front(t_rm_node **lst, t_rm_node *new_node);
void		rm_node_clear_list(t_rm_node **lst, void (*del)(void *));
t_rm_node	*rm_node_unlink(t_rm_node **head, void *target, int (*match)(void *,
					void *));
t_rm_node	*rm_node_find(t_rm_node **head, void *target, int (*match)(void *,
					void *));
int			match_resource(void *node_ptr, void *target_resource_pointer);
int			match_manager(void *node_ptr, void *target_manger_name_ptr);
void		rm_node_del_resource(void *node_ptr);

void		*safe_malloc(size_t size);
void		*track_malloc(size_t size);

void		report_error(char *message);
void		error(char *message);

t_rm_node	**managers_get_instance(void);
void		manager_add(char *name);
void		manager_free(void);
void		manager_free_everything(void);

void		resource_track(void *resource, void (*cleanup)(void *));
void		resource_free(void *resource);
void		resource_transfer(void *resource_pointer,
				char *target_manager_name);

#endif

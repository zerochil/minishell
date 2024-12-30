/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 01:07:53 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 03:30:36 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_management.h"


typedef struct s_rm_node t_rm_node;
struct s_rm_node
{
	union 
	{
		struct
		{
			void				*pointer;
			void				(*cleanup)(void *);
		} resource;
		struct
		{
			char				*name;
			t_rm_node			*resources;
		} manager;

	};
	t_rm_node	*next;
};


void	*safe_malloc(size_t size)
{
	void	*allocated_space;

	allocated_space = ft_calloc(size, 1);
	if (allocated_space == NULL)
		error("Safe Callocation failed; BUY MORE RAM!!!");
	return (allocated_space);
}

void	report_error(char *message)
{
	if (message && *message != '\0')
		ft_putendl_fd(message, 2);
}
void	free_all_resources(void);

void	error(char *message)
{
	if (message && *message != '\0')
		ft_putendl_fd(message, 2);
	free_all_resources();
	exit(1);
}


t_rm_node	*rm_node_new_manager(char *name)
{
	t_rm_node	*node;
	node = safe_malloc(sizeof(t_rm_node));
	node->manager.name = name;
	return node;
}

void	rm_node_add_front(t_rm_node **lst, t_rm_node *new_node)
{
	if (lst == NULL || new_node == NULL)
		return ;
	new_node->next = *lst;
	*lst = new_node;
}

void	rm_node_del_resource(void *node_ptr)
{
	t_rm_node	*node;

	node = node_ptr;
	node->resource.cleanup(node->resource.pointer);
	free(node);
}

void	rm_node_clear_list(t_rm_node **lst, void (*del)(void *))
{
	t_rm_node	*next;
	t_rm_node	*tmp;

	if (lst == NULL || del == NULL)
		return ;
	next = *lst;
	while (next)
	{
		tmp = next->next;
		del(next);
		next = tmp;
	}
	*lst = NULL;
}


t_rm_node	**managers_get_instance(void)
{
	static t_rm_node	*head;
	t_rm_node		*node;
	if (head == NULL)
		head = rm_node_new_manager("global");
	return (&head);
}

void	manager_add(char *name)
{
	t_rm_node		**managers_list;
	t_rm_node	*new_manager;

	managers_list = managers_get_instance();
	rm_node_add_front(managers_list, rm_node_new_manager(name));
}

void	manager_free(void)
{
	t_rm_node		**managers_list;
	t_rm_node		*manager_node;

	managers_list = managers_get_instance();
	if (*managers_list == NULL)
		return ;
	manager_node = *managers_list;
	rm_node_clear_list(&manager_node->manager.resources, rm_node_del_resource);
	*managers_list = manager_node->next;
	free(manager_node);
}

t_rm_node	*ft_lstunlink(t_rm_node **head, void *target, int (*match)(void *, void*))
{
	t_rm_node	**indirect = head;
	t_rm_node	*node_to_unlink;

	if (!head || !match)
		return (NULL);
	while (*indirect)
	{
		if (match(*indirect, target))
		{
			node_to_unlink = *indirect;
			*indirect = node_to_unlink->next;
			node_to_unlink->next = NULL;
			return (node_to_unlink);
		}
		indirect = &(*indirect)->next;
	}
	return (NULL);
}


t_rm_node	*ft_lstfind(t_rm_node **head, void *target, int (*match)(void *, void*))
{
	t_rm_node	**indirect = head;

	if (!head || !match)
		return (NULL);
	while (*indirect)
	{
		if (match(*indirect, target))
			return (*indirect);
		indirect = &(*indirect)->next;
	}
	return (NULL);
}

int	match_resource(void *node_ptr, void *target_resource_pointer)
{
	t_rm_node	*node;

	node = node_ptr;
	return (node->resource.pointer == target_resource_pointer);
}

int	match_manager(void *node_ptr, void *target_manger_name_ptr)
{
	t_rm_node	*node;
	char		*target_manger_name;

	target_manger_name = target_manger_name_ptr;
	node = node_ptr;
	return (ft_strcmp(node->manager.name, target_manger_name) == 0);
}


void	manager_transfer_resource(void *resource_pointer, char *target_manager_name)
{
	t_rm_node		**managers_list;
	t_rm_node		*manager_node;
	t_rm_node		*resource_node;
	t_rm_node		*target_manager_node;

	managers_list = managers_get_instance();
	if (*managers_list == NULL)
		return ;
	manager_node = *managers_list;
	target_manager_node = ft_lstfind(managers_list, target_manager_name, match_manager);
	if (target_manager_node == NULL)
		return ;
	resource_node = ft_lstunlink(&manager_node->manager.resources, resource_pointer, match_resource);
	if (resource_node == NULL)
		return ;
	rm_node_add_front(&target_manager_node->manager.resources, resource_node);
}

void	track_resource(void *resource, void (*cleanup)(void *))
{
	// TODO: GUARD: what if we track the sam resource multiple times??
	t_rm_node	**manager_node;
	t_rm_node	*new_resource;

	if (cleanup == NULL)
		error("Failed to track resource, no cleanup function");
	manager_node = managers_get_instance();
	new_resource = malloc(sizeof(t_rm_node));
	if (new_resource == NULL)
	{
		cleanup(resource);
		error("Failed to allocate space for resource tracker");
	}
	new_resource->resource.pointer = resource;
	new_resource->resource.cleanup = cleanup;
	rm_node_add_front(&(*manager_node)->manager.resources, new_resource);
}

void	*track_malloc(size_t size)
{
	void	*allocated_space;

	allocated_space = safe_malloc(size);
	track_resource(allocated_space, free);
	return (allocated_space);
}
	
void	free_resource(void *resource)
{
	t_rm_node   **manager_node;
	t_rm_node	**indirect;
	t_rm_node	*to_remove;

	manager_node = managers_get_instance();
	if (manager_node == NULL || *manager_node == NULL)
		return ;
	indirect = &(*manager_node)->manager.resources;
	while (*indirect)
	{
		if ((*indirect)->resource.pointer == resource)
		{
			to_remove = *indirect;
			*indirect = (*indirect)->next;
			to_remove->resource.cleanup(to_remove->resource.pointer);
			free(to_remove);
			return ;
		}
		indirect = &(*indirect)->next;
	}
}



void	close_fd(void *fd_ptr)
{
	int	fd;

	fd = (int)(intptr_t)fd_ptr;
	if (fd >= 0)
		close(fd);
}

int	track_open(char *pathname, int flags, mode_t mode, char *error_message)
{
	int	fd;

	fd = open(pathname, flags, mode);
	if (fd < 0)
		error(error_message);
	track_resource((void *)(intptr_t)fd, close_fd);
	return (fd);
}

void	free_all_resources(void)
{
	t_rm_node	**manager;

	manager = managers_get_instance();
	while (*manager)
		manager_free();
}

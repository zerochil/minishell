/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   containers_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 15:00:32 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_containers_dependencies.h"

void	*container_grow(void *old_data, size_t old_cap, size_t new_cap,
		size_t elem_size)
{
	void	*new_data;

	new_data = track_malloc(new_cap * elem_size);
	if (old_data)
	{
		ft_memcpy(new_data, old_data, old_cap * elem_size);
		resource_free(old_data);
	}
	return (new_data);
}

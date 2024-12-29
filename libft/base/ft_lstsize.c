/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:07:27 by rrochd            #+#    #+#             */
/*   Updated: 2024/09/10 15:07:28 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*next;
	size_t	size;

	next = lst;
	size = 0;
	while (next)
	{
		size++;
		next = next->next;
	}
	return (size);
}

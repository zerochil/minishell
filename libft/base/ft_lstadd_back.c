/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:11:03 by rrochd            #+#    #+#             */
/*   Updated: 2024/11/26 16:26:21 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	if (lst == NULL || new_node == NULL)
		return ;
	if (*lst == NULL)
		*lst = new_node;
	else
	{
		last = ft_lstlast(*lst);
		last->next = new_node;
	}
}

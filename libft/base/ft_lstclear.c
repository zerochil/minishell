/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:59:32 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/21 23:52:47 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next;
	t_list	*tmp;

	if (lst == NULL || del == NULL)
		return ;
	next = *lst;
	while (next)
	{
		tmp = next->next;
		ft_lstdelone(next, del);
		next = tmp;
	}
	*lst = NULL;
}

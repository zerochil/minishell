/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:32:58 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/29 12:36:07 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char  string_at(t_string *string, size_t index)
{
  if (string == NULL || index > string->size)
  {
    report_error("string_at: error");
    return (-1);
  }
  return (string->data[index]);
}

int  string_match(t_string *string, char *to_find, size_t start)
{
	if (string == NULL || start > string->size || to_find == NULL)
	{
		report_error("string_match: error");
		return (-1);
	}
	return (ft_strncmp(string->data+start, to_find, ft_strlen(to_find)) == 0);
}

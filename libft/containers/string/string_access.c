/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 22:32:58 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/22 12:15:54 by rrochd           ###   ########.fr       */
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

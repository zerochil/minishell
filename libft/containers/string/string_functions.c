/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:57:55 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/22 18:43:12 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

void	string_init(t_string *string)
{
	string->data = track_malloc((INITIAL_CAPACITY + 1) * sizeof(char));
	string->size = 0;
	string->capacity = INITIAL_CAPACITY;
  string->peek = 0;
}

void	string_set(t_string *string, char *new_text)
{
  size_t  new_text_length;

  if (string == NULL || new_text == NULL)
  {
    report_error("string_at: error");
    return ;
  }
  new_text_length = ft_strlen(new_text);
  string->size = new_text_length;
  string_ensure_capacity(string, new_text_length);
  ft_memcpy(string->data, new_text, new_text_length + 1);
}

void	string_destroy(t_string *string)
{
	free_resource(string->data);
	string->data = NULL;
	string->size = 0;
	string->capacity = 0;
  string->peek = 0;
}

void string_ensure_capacity(t_string *string, size_t needed_capacity)
{
	size_t new_cap;

	needed_capacity = needed_capacity + 1;
    if (needed_capacity >= string->capacity)
    {
        new_cap = string->capacity;
        while (new_cap < needed_capacity)
            new_cap *= 2;
        
        string->data = container_grow(string->data, string->capacity, new_cap, 
                                    sizeof(char));
        string->capacity = new_cap;
    }
}

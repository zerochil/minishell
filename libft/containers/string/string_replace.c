/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_replace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:37:36 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/25 11:37:37 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

void	string_replace(t_string *string, const char *old_text,
		const char *new_text)
{
	char	*occurunce;

	if (string == NULL || old_text == NULL || new_text == NULL)
	{
		report_error("string_replace: error");
		return ;
	}
	if (*old_text == '\0')
		return ;
	while (true)
	{
		occurunce = ft_strnstr(string->data, old_text, string->size);
		if (occurunce == NULL)
			break ;
		string_segment_replace(string, occurunce - string->data,
			ft_strlen(old_text), new_text);
	}
}

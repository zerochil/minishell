/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 09:12:44 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 10:57:01 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "field.h"

static t_string	*get_field_mask(char *str)
{
	t_string	*mask;
	size_t		str_length;
	size_t		i;
	char		in_quote;

	str_length = ft_strlen(str);
	mask = track_malloc(sizeof(t_string));
	string_init(mask);
	string_ensure_capacity(mask, str_length);
	mask->size = str_length;
	in_quote = '\0';
	i = 0;
	while (str[i])
	{
		if (in_quote == '\0' && (str[i] == '\'' || str[i] == '"'))
			in_quote = str[i];
		else if (in_quote == str[i])
			in_quote = '\0';
		else if (in_quote == '\'')
			mask->data[i] |= SINGLE_QUOTED;
		else if (in_quote == '"')
			mask->data[i] |= DOUBLE_QUOTED;
		i++;
	}
	return (mask);
}

t_field	*field_init(char *token_value, char *mask)
{
	t_field	*field;

	if (token_value == NULL)
	{
		report_error("field_init: error");
		token_value = "";
	}
	field = track_malloc(sizeof(t_field));
	field->value = track_malloc(sizeof(t_string));
	string_init(field->value);
	string_set(field->value, token_value);
	field->mask = get_field_mask(token_value);
	if (mask != NULL)
		ft_memcpy(field->mask->data, mask, field->value->size);
	return (field);
}

void	field_set(t_field *field, char *value, unsigned char mask)
{
	if (field == NULL)
	{
		report_error("Error: field_set");
		return ;
	}
	string_set(field->value, value);
	string_set(field->mask, value);
	field_peek_reset(field);
	ft_memset(field->mask->data, mask, field->mask->size);
}

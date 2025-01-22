/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:49:01 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 12:36:43 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "field.h"

bool	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	skip_ifs(t_field *field)
{
	while (is_ifs(string_peek(field->value))
		&& (string_peek(field->mask) == EXPANDED))
		field_shift(field);
}

t_field	*field_copy(t_field *field)
{
	t_field	*copy;

	if (!field)
	{
		report_error("field_copy: error");
		return (NULL);
	}
	copy = field_init(field->value->data, field->mask->data);
	return (copy);
}

void	field_shift(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_shift");
		return ;
	}
	string_shift(field->value);
	string_shift(field->mask);
}

bool	field_remove_original_quotes(t_field *field)
{
	size_t	old_size;
	char	c;
	char	m;

	old_size = field->value->size;
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break ;
		else if ((c == '\'' || c == '"' ) && m == ORIGINAL)
			field_shift_at_peek(field);
		else
			field_peek_advance(field);
	}
	return (field->value->size != old_size);
}

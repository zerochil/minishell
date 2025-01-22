/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:46:47 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 10:47:46 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "field.h"

void	field_peek(t_field *field, char *c, char *m)
{
	if (field == NULL || c == NULL || m == NULL)
	{
		report_error("Error: field_peek");
		return ;
	}
	*c = string_peek(field->value);
	if (field->mask)
		*m = string_peek(field->mask);
}

void	field_peek_set(t_field *field, size_t peek)
{
	if (field == NULL || peek > field->value->size)
	{
		report_error("Error: field_peek_set");
		return ;
	}
	field->value->peek = peek;
	if (field->mask)
		field->mask->peek = peek;
}

void	field_peek_advance(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_peek_advance");
		return ;
	}
	string_peek_advance(field->value);
	if (field->mask)
		string_peek_advance(field->mask);
}

void	field_peek_reset(t_field *field)
{
	if (field == NULL)
	{
		report_error("Error: field_peek");
		return ;
	}
	string_peek_reset(field->value);
	if (field->mask)
		string_peek_reset(field->mask);
}

char	field_shift_at_peek(t_field *field)
{
	char	c;

	if (field == NULL)
	{
		report_error("Error: field_shift_at_peek");
		return (-1);
	}
	c = string_peek(field->value);
	string_segment_remove(field->value, field->value->peek, 1);
	if (field->mask)
		string_segment_remove(field->mask, field->mask->peek, 1);
	return (c);
}

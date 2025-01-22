/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:00:48 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 11:50:47 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parameter_expansion.h"

static void	expand_mask_at_peek(t_string *mask, int key_length,
		int value_length, int value_mask)
{
	char	*tmp;

	tmp = safe_malloc((value_length + 1) * sizeof(char));
	ft_memset(tmp, value_mask, value_length);
	string_segment_replace(mask, mask->peek, key_length, tmp);
	free(tmp);
}

static bool	expand_parameter_in_field(t_field *field)
{
	char	*parameter_name;
	size_t	dolar_position;
	ssize_t	value_length;
	int		mask;

	dolar_position = field->value->peek;
	parameter_name = get_parameter_name(field->value, dolar_position);
	value_length = expand_parameter(field->value, parameter_name);
	if (value_length < 0)
		return (false);
	mask = EXPANDED | (field->mask->data[dolar_position] & DOUBLE_QUOTED);
	expand_mask_at_peek(field->mask, ft_strlen(parameter_name),
		value_length, mask);
	return (true);
}

static bool	expand_field_parameter(t_field *field)
{
	char	c;
	char	m;
	bool	has_expanded;
	bool	should_field_split;

	should_field_split = false;
	field_peek_reset(field);
	while (true)
	{
		field_peek(field, &c, &m);
		if (c == '\0')
			break ;
		if (c == '$' && (m & SINGLE_QUOTED) == 0)
		{
			has_expanded = expand_parameter_in_field(field);
			if ((m & DOUBLE_QUOTED) == 0)
				should_field_split = has_expanded;
			field->mask->peek = field->value->peek;
		}
		else
			field_peek_advance(field);
	}
	return (should_field_split);
}

void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	t_field		*field;

	token = token_ptr;
	field = array_get(token->fields, 0);
	token->should_field_split = expand_field_parameter(field);
}

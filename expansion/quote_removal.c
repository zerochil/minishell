/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:29:30 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:59:24 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	single_field_quote_removal(void *field_ptr)
{
	t_field	*field;

	field = field_ptr;
	field_remove_original_quotes(field);
}

void	quote_removal(void *token_ptr)
{
	t_token	*token;

	token = token_ptr;
	array_do(token->fields, single_field_quote_removal);
}

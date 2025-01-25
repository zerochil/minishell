/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:30:30 by inajah            #+#    #+#             */
/*   Updated: 2025/01/25 08:57:31 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

bool	is_export_command(t_array *tokens)
{
	t_token	*token;

	token = array_get(tokens, 0);
	return (ft_strcmp("export", token->value) == 0);
}

bool	is_assingment_word(t_token *token)
{
	char	*key;

	key = get_key(token->value);
	if (key == NULL)
		return (false);
	free(key);
	return (true);
}

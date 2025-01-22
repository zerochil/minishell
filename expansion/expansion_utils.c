/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:30:30 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 14:59:53 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

bool	is_export_command(t_token *token)
{
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

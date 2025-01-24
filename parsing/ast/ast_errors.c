/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 07:54:26 by rrochd            #+#    #+#             */
/*   Updated: 2025/01/23 07:54:27 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ast.h>

char	*syntax_error(char *message)
{
	static char	*error_message;

	if (message == NULL)
		error_message = NULL;
	else if (*message != '\0')
		error_message = message;
	return (error_message);
}

bool	check_syntax_error(bool condition, char *message)
{
	if (condition)
	{
		syntax_error(message);
		return (true);
	}
	return (false);
}

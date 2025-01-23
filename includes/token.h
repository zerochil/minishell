/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:43:07 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 11:44:33 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <libft.h>

typedef struct s_token
{
	int			type;
	char		*value;
	bool		should_field_split;
	t_array		*fields; 
}				t_token;

#endif

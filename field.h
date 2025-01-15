/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 20:53:34 by inajah            #+#    #+#             */
/*   Updated: 2025/01/14 20:57:47 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIELD_H
# define FIELD_H

typedef struct s_field
{
	t_string	*value;
	t_string	*mask;
}	t_field;


t_field	*field_init(char *token_value, char *mask);

#endif


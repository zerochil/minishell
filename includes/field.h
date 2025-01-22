/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inajah <inajah@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 20:53:34 by inajah            #+#    #+#             */
/*   Updated: 2025/01/22 12:32:26 by inajah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIELD_H
# define FIELD_H

# include <stdlib.h>
# include <stdbool.h>
# include "libft.h" 

typedef struct s_field
{
	t_string	*value;
	t_string	*mask;
}	t_field;

enum e_field_mask_value
{
	ORIGINAL = 0,
	EXPANDED = 1,             
	SINGLE_QUOTED = 2,
	DOUBLE_QUOTED = 4,
};

t_field		*field_init(char *token_value, char *mask);
void		field_set(t_field *field, char *value, unsigned char mask);
void		field_split(t_array *fields);
void		field_set(t_field *field, char *value, unsigned char mask);
char		field_shift_at_peek(t_field *field);

void		field_peek(t_field *field, char *c, char *m);
void		field_peek_set(t_field *field, size_t peek);
void		field_peek_advance(t_field *field);
void		field_peek_reset(t_field *field);

t_field		*field_copy(t_field *field);
bool		field_remove_original_quotes(t_field *field);
void		field_shift(t_field *field);
bool		is_ifs(char c);
void		skip_ifs(t_field *field);


#endif


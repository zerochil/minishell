#ifndef CONTEXT_H
#define CONTEXT_H

#include "libft/libft.h"

typedef struct s_context
{
	t_array	*ast_root_list;
	t_array *pids;
} t_context;

t_context *get_context_instance();

#endif

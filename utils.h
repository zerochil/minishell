#ifndef UTILS_H
#define UTILS_H

#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

void	destroy_context(void);
bool is_valid_string(bool is_valid(char, int), char *str, size_t length);
bool is_valid_identifier(char c, int i);

#endif

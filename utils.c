#include "utils.h"

void	destroy_context(void)
{
	manager_free_everything();
	rl_clear_history();
}


bool is_valid_string(bool is_valid(char, int), char *str, size_t length)
{
	size_t i;

	if (str == NULL || is_valid == NULL || length == 0)
		return (false);
	if (length > ft_strlen(str))
		length = ft_strlen(str);
	i = 0;
	while (i < length)
	{
		if (is_valid(str[i], i) == false)
			return (false);
		i++;
	}
	return (true);
}

bool is_valid_identifier(char c, int i)
{
	if (i == 0)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

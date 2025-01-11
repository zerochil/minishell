#include "utils.h"

void	destroy_context(void)
{
	manager_free_everything();
	rl_clear_history();
}


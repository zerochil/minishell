#include <stdlib.h>

void	track_resource(void *resource, void (*cleanup)(void *));
void	free_all_resources(void);
void	manager_free(void);
void	manager_add(char *name);
void	*track_malloc(size_t size);
void	manager_transfer_resource(void *resource_pointer, char *target_manager_name);






int main()
{
	manager_add("xx");
	char *ptr2 = track_malloc(321);
	char *ptr1 = malloc(1);
	track_resource(ptr1, free);
	manager_transfer_resource(ptr1, "global");

	manager_free();
	manager_free();
}

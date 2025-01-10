#include "libft/libft.h"
#include <stdio.h>

void print(void *str_ptr)
{
	char *str = str_ptr;
	printf("[%s]\n", str);
}

int main()
{
	t_array arr;
	array_init(&arr);
	for (int i = 0; i < 129; i++)
	{
		char *str = ft_itoa(i);
		resource_track(str, free);
		array_insert(&arr, 0, str);
	}
	array_do(&arr, print);
	printf("%ld\n", arr.size);
	printf("%ld\n", arr.capacity);
	manager_free_everything();
}

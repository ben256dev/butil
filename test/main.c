#define BUTIL_IMPLEMENTATION
#include "../butil.h"

int main(int argc, char* argv[])
{
	Allocator alloc = {
        .alloc   = da_alloc,
        .free    = da_free,
        .context = NULL,
    };

	int *my_array = array(int, &alloc);
	array_append(my_array, 1);
	array_append(my_array, 2);
	array_append(my_array, 3);
	array_append(my_array, 4);
	array_append(my_array, 5);
	array_append(my_array, 6);
	array_append(my_array, 7);
	array_append(my_array, 8);
	array_append(my_array, 9);
	array_append(my_array, 10);
	array_append(my_array, 11);

	for (int i = 0; i < array_length(my_array); i++) {
		printf("%d\n", my_array[i]);
	}
	putchar('\n');

	array_remove(my_array, 3);

	for (int i = 0; i < array_length(my_array); i++) {
		printf("%d\n", my_array[i]);
	}

	return 0;
}

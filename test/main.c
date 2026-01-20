#define BUTIL_IMPLEMENTATION
#include "../butil.h"

int main(int argc, char* argv[])
{
	int *my_array = array(int);
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

	array_foreach_ptr(x, my_array)
		printf("%d\n", *x);
	putchar('\n');

	array_remove(my_array, 3);

	array_foreach(x, my_array)
		printf("%d\n", x);
	putchar('\n');

	array_remove_unordered(my_array, 3);

	array_foreach_const(x, my_array)
		printf("%d\n", *x);

	return 0;
}

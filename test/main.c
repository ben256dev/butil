#define BUTIL_IMPLEMENTATION
#include "../butil.h"

int main(int argc, char* argv[])
{
	float *my_array = array(float);
	array_append(my_array, 1.0f);
	array_append(my_array, 2.0f);
	array_append(my_array, 3.0f);
	array_append(my_array, 4.0f);
	array_append(my_array, 5.0f);
	array_append(my_array, 6.0f);
	array_append(my_array, 7.0f);
	array_append(my_array, 8.0f);
	array_append(my_array, 9.0f);
	array_append(my_array, 10.0f);
	array_append(my_array, 11.0f);

	array_foreach(float, x, my_array)
		printf("%f\n", x);
	putchar('\n');

	array_remove(my_array, 3);

	array_foreach(float, x, my_array)
		printf("%f\n", x);
	putchar('\n');

	array_remove_unordered(my_array, 3);

	array_foreach(float, x, my_array)
		printf("%f\n", x);
	putchar('\n');

	return 0;
}

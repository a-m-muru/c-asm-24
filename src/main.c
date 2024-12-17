#include "bf_interpretator.h"
#include "bf_memory.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#define TEST_IMPLEMENTATION
#include "test.h"

FILE *get_print_output()
{
	return stdout;
}

int main(int argc, char **argv)
{
	bool print = false;
	if (argc < 2) {
		fprintf(stderr,
			"Interpretaator vajab teise argumendina programmi\n");
		return 1;
	}
	if (argc >= 3
	    && (strcmp("--print", argv[2]) == 0
		|| strcmp("-p", argv[2]) == 0)) {
		print = true;
	}
	mem_init();

	interpret(argv[1], print);

	// bf_test();

#ifdef DEBUG
	mem_print_debug();
#endif //DEBUG

	return 0;
}

#ifndef __TEST_H__
#define __TEST_H__

#include "bf_instructions.h"
#include "bf_interpretator.h"
#include "bf_memory.h"

void bf_test();

//#define TEST_IMPLEMENTATION
#ifdef TEST_IMPLEMENTATION

extern int8_t *_bf_memory;
extern uint32_t _bf_mem_pointer;

void bf_test()
{
	int32_t index = 10;
	Instruction *increment = new_increment(2);
	puts("---TESTING---");
	puts(increment != NULL ? "making instruction worked" :
				 "making instruction failed");
	increment_run(increment, &index);
	printf(mem_get() == 2 ? "incrementing worked (is %d)" :
				"incrementing didn't work (is %d)",
	       mem_get());
	puts("");
	printf("testing index incrementation: %s (ix = %d)",
	       index == 11 ? "OK" : "error", index);
	Instruction *move = new_move(14);
	move_run(move, &index);
	puts("");
	printf("testing pointer move: %s (ix = %u)",
	       _bf_mem_pointer == 14 ? "OK" : "error", _bf_mem_pointer);

	puts("");
}

#endif // TEST_IMPLEMENTATION

#endif // __TEST_H__

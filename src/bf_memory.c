#include "bf_memory.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int8_t _bf_memory[MEMORY_SIZE];
uint32_t _bf_mem_pointer = 0;
bool _mem_initialised = false;

static void _mem_init_check(void)
{
	if (!_mem_initialised) {
		perror("sisemine viga: mälu hõivamata (mem_init puudu)\n");
		exit(1);
	}
}

int32_t mem_add(int32_t amount)
{
	_mem_init_check();
	_bf_memory[_bf_mem_pointer] += amount;
	return _bf_memory[_bf_mem_pointer];
}

int32_t mem_move(int32_t poses)
{
	_mem_init_check();

	int32_t after = (int32_t)_bf_mem_pointer + poses;
	if (after > (int32_t)MEMORY_SIZE)
		_bf_mem_pointer = _bf_mem_pointer + poses - MEMORY_SIZE;
	else if (after < 0) _bf_mem_pointer = MEMORY_SIZE + poses;
	else _bf_mem_pointer = _bf_mem_pointer + poses;

	return _bf_mem_pointer;
}

void mem_init(void)
{
	for (size_t i = 0; i < MEMORY_SIZE; i++) {
		_bf_memory[i] = 0;
	}
	_bf_mem_pointer = 0;
	_mem_initialised = true;
}

int32_t mem_inc(void)
{
	return mem_add(1);
}

int32_t mem_dec(void)
{
	return mem_add(-1);
}

int32_t mem_left(void)
{
	return mem_move(-1);
}

int32_t mem_right(void)
{
	return mem_move(1);
}

int32_t mem_get(void)
{
	_mem_init_check();
	return (int32_t)_bf_memory[_bf_mem_pointer];
}

int32_t mem_set(int8_t v)
{
	_mem_init_check();
	_bf_memory[_bf_mem_pointer] = v;
	return _bf_memory[_bf_mem_pointer];
}

int8_t mem_get_input(void)
{
	int32_t c = getc(stdin);
	mem_set((int8_t)c);
	return (int8_t)c;
}

void mem_print_debug(void)
{
	printf("index: %d mem [%d .. %d] ", _bf_mem_pointer, _bf_mem_pointer,
	       (_bf_mem_pointer + 9) % MEMORY_SIZE);
	for (int32_t i = 0; i < 10; i++) {
		printf("%d ", _bf_memory[(_bf_mem_pointer + i) % MEMORY_SIZE]);
	}
	printf("\n");
}

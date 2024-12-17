#ifndef __BF_MEMORY_H__
#define __BF_MEMORY_H__

#include <stdint.h>
#include <stdio.h>

#define MEMORY_SIZE 30000u

int32_t mem_add(int32_t amount);
int32_t mem_move(int32_t poses);
void mem_init(void);
int32_t mem_inc(void);
int32_t mem_dec(void);
int32_t mem_left(void);
int32_t mem_right(void);
int32_t mem_get(void);
int32_t mem_set(int8_t v);
int8_t mem_get_input(void);
void mem_print_debug(void);

#endif // __BF_MEMORY_H__

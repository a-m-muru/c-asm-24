#ifndef __PINU_H__
#define __PINU_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
	uint32_t len;

	uint32_t size;

	int32_t *mem;
} Stack;

Stack *construct_stack();

void stack_push(Stack *s, int32_t el);
int32_t stack_pop(Stack *s);
int32_t stack_peek(Stack *s);
bool stack_is_empty(Stack *s);

void stack_print(Stack *s, FILE *stream);

void destroy_stack(Stack **s);

#endif // __PINU_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pinu.h"

Stack *construct_stack()
{
	Stack *s = malloc(sizeof(Stack));
	s->mem = NULL;
	s->len = 0;
	s->size = 0;
	return s;
}

void stack_push(Stack *s, int32_t el)
{
	if (s->size == 0 || s->mem == NULL) {
		// mälu pole allokeeritud

		s->size = 1;
		s->mem = malloc(sizeof(int32_t) * s->size);

		// veahaldus
		if (s->mem == NULL) {
			fprintf(stderr, "PINUle mälu ei suudetud eraldada\n");
			exit(1);
		}

		s->len = 0;
	} else if (s->len == s->size) {
		// suurendame mälu 2x
		uint32_t temp_size = s->size * 2;
		int32_t *temp_mem = NULL;

		temp_mem = realloc(s->mem, temp_size * sizeof(int32_t));
		if (temp_mem == NULL) {
			fprintf(stderr, "PINU mälu ei suudetud suurendada\n");
			// exit(1);
		}

		s->mem = temp_mem;
		s->size = temp_size;
	}

	s->mem[s->len] = el;
	s->len++;
}

int32_t stack_pop(Stack *s)
{
	if (s->len == 0 || s->mem == NULL) {
		fprintf(stderr, "PINU on tühi\n");
		return -1;
	}

	int32_t top_element = s->mem[s->len - 1];
	//s->mem[s->len] = 0;
	s->len--;

	if (s->len == 0) {
		// tühja pinu mälu kustutame
		free(s->mem);
		s->mem = NULL;

	} else if (s->len <= s->size / 4) {
		// peaks ruumi eemaldama

		uint32_t temp_size = s->size / 2;
		int32_t *temp_mem = NULL;

		temp_mem = realloc(s->mem, temp_size * sizeof(int32_t));
		if (temp_mem == NULL) {
			fprintf(stderr, "PINU mälu ei suudetud vähendada\n");
			exit(1);
		}

		s->mem = temp_mem;
		s->size = temp_size;
	}

	return top_element;
}

int32_t stack_peek(Stack *s)
{
	if (s->len == 0 || s->mem == NULL) {
		fprintf(stderr, "PINU on tühi\n");
		return -1;
	}
	int32_t top_element = s->mem[s->len - 1];
	return top_element;
}

bool stack_is_empty(Stack *s)
{
	return s->len == 0;
}

void stack_print(Stack *s, FILE *stream)
{
	fprintf(stream, "[");
	for (size_t i = 0; i < s->len; i++) {
		int32_t elmelm = s->mem[i];
		fprintf(stream, "%d", elmelm);
		if (i < s->len - 1) {
			fprintf(stream, ", ");
		}
	}
	fprintf(stream, "]\n");
}

void destroy_stack(Stack **s)
{
	(*s)->len = 0;
	free((*s)->mem);
	(*s)->size = 0;
	*s = NULL;
}

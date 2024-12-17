#include "bf_instructions.h"
#include "bf_interpretator.h"
#include "bf_memory.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE *get_print_output();

void parse(Instruction **inst_arr, const char *program)
{
	uint32_t i = 0;

	int32_t loop_stack[strlen(program)];
	uint32_t lsp = 0; // "loop stack pointer"

	while (program[i] != 0) {
		/* Algväärtustame kõik instruktsioonid eelnevalt NULL'iga. */
		inst_arr[i] = NULL;
		switch (program[i]) {
		case '+': {
			inst_arr[i] = new_increment(1);
			break;
		}

		case '-': {
			inst_arr[i] = new_increment(-1);
			break;
		}

		case '>': {
			inst_arr[i] = new_move(1);
			break;
		}

		case '<': {
			inst_arr[i] = new_move(-1);
			break;
		}

		case '.': {
			inst_arr[i] = new_output();
			break;
		}
		case ',': {
			inst_arr[i] = new_input();
			break;
		}

		case '[': {
			inst_arr[i] = new_begin_loop();
			loop_stack[lsp++] = i;
			break;
		}

		case ']': {
			if (lsp <= 0) {
				fprintf(stderr, "Tsükli lõpp vales kohas\n");
				exit(EXIT_FAILURE);
			}
			int32_t begin_index = loop_stack[--lsp];
			inst_arr[i] = new_end_loop(begin_index);

			/* Uuendame ka tsükli algust, et seal oleks olemas info, kus asub tsükli lõpp! */
			inst_arr[begin_index]->loop_forward_index = i;

			break;
		}

		case '#': {
			inst_arr[i] = new_print();
			break;
		}

		default:
			/* Ignoreerime sümboleid, mida me ei tunne. */
			break;
		}
		i++;
	}

	if (lsp != 0) {
		fprintf(stderr, "Tüskkel tasakaalustamata\n");
		exit(EXIT_FAILURE);
	}
}

void interpret(const char *program, bool do_print_asm)
{
	/* Leiame programmi lähtekoodi pikkuse. */
	int32_t program_len = strlen(program);

	/* Teeme massiivi, mis hoiab viitasid, mida on kokku program_len tükku.
       Viitade algväärtustamine toimub parse() funktsioonis. Massiivi pikkus on
       võetud varuga */
	Instruction **inst_arr = malloc(sizeof(Instruction *) * program_len);

	/* Parsime sisendprogrammi, mille tulemus salvestatakse inst_arr massiivi. */
	parse(inst_arr, program);

	/* Käivitame programmi. */
	if (do_print_asm) print_asm(inst_arr, program_len);
	else run(inst_arr, program_len);

	/** Mälu vajab vabastamist! **/
	for (int32_t i = 0; i < program_len; i++) {
		if (inst_arr[i] == NULL) continue;
		free(inst_arr[i]);
	}
	free(inst_arr);
}

void run(Instruction **instructions, int32_t len_instructs)
{
	int32_t ix = 0;
	for (;;) {
		if (ix < 0 || ix >= len_instructs) break;
		if (instructions[ix] == NULL) {
			ix++;
			continue;
		}
		instructions[ix]->run(instructions[ix], &ix);
	}
}

void print_asm(Instruction **instructions, int32_t len_instructs)
{
	int32_t ix = 0;
	FILE *o = get_print_output();
	fprintf(o, "global main\n"
		   "extern mem_add\n"
		   "extern mem_init\n"
		   "extern mem_move\n"
		   "extern mem_inc\n"
		   "extern mem_dec\n"
		   "extern mem_left\n"
		   "extern mem_right\n"
		   "extern mem_get\n"
		   "extern mem_set\n"
		   "extern mem_get_input\n"
		   "extern mem_print_debug\n\n"
		   "extern putchar\n\n");
	fprintf(o, "section .text\n");
	fprintf(o, "main:\n");
	fprintf(o, "\t; initsialiseerime mälu\n");
	fprintf(o, "\tcall mem_init\n\n");
	for (;;) {
		if (ix < 0 || ix >= len_instructs) break;
		if (instructions[ix] == NULL) {
			ix++;
			continue;
		}
		instructions[ix]->print(instructions[ix], &ix);
	}
	fprintf(o, "\tret");
}

#include "bf_instructions.h"
#include "bf_memory.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

FILE *get_print_output();

static Instruction *_new_instruction()
{
	Instruction *inc = NULL;
	inc = malloc(sizeof(*inc));
	if (inc == NULL) {
		fprintf(stderr, "Mälu hõivamine nurjus");
		abort();
		return NULL;
	}
	return inc;
}

/* Konstruktor funktsioon BF_increment_new loob uue struktuuri, mis suurendab
   aktiivse mäluala väärtust vastavalt parameetrina antud väärtusele, mis võib
   olla ka negatiivne.
*/
Instruction *new_increment(int32_t increment)
{
	/* Kontrollime väärtust, et vältida hilisemaid vigu. */
	if (increment == 0) {
		fprintf(stderr, "Ei liida 0ga");
		/* Sisuliselt tagastab nüüd funktsioon NULL'i. */
		return NULL;
	}

	Instruction *inc = _new_instruction();

	inc->increment = increment;
	inc->run = increment_run;
	inc->print = increment_print;
	return inc;
}

/* Funkctioon BF_increment_run on kood, mida käivitatakse konkreetse
   instruktsiooni käivitamisel.
*/
void increment_run(Instruction *instruction, int32_t *index)
{
	/* Suurendame mälu väärtust vastavalt konstruktoris seatud väärtusele! */
	mem_add(instruction->increment);

	/* Suurendame instruktsiooniloendurit ühe võrra. */
	++*index;
}

void increment_print(Instruction *instruction, int32_t *index)
{
	FILE *file = get_print_output();
	fprintf(file, "\t;;; increment\n");
	fprintf(file,
		"\t; magasini suurendamise instruktsioon\n"
		"\tpush dword %d\n",
		instruction->increment);
	fprintf(file, "\tcall mem_add\n");
	fprintf(file, "\t; magasini tühjendamine\n"
		      "\tadd esp, 4\n"
		      "\n");
	++*index;
}

Instruction *new_move(int32_t move)
{
	/* Kontrollime väärtust, et vältida hilisemaid vigu. */
	if (move == 0) {
		fprintf(stderr, "Ei liigu 0 kohta");
		/* Sisuliselt tagastab nüüd funktsioon NULL'i. */
		return NULL;
	}
	Instruction *inc = _new_instruction();

	inc->number_of_positions = move;
	inc->run = move_run;
	inc->print = move_print;
	return inc;
}

void move_run(Instruction *instruction, int32_t *index)
{
	mem_move(instruction->number_of_positions);
	++*index;
}

void move_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; move\n");
	fprintf(o,
		"\t; pc liigutamise instruktsioon\n"
		"\tpush dword %d\n",
		instruction->number_of_positions);
	fprintf(o, "\tcall mem_move\n");
	fprintf(o, "\t; magasini tühjendamine\n"
		   "\tadd esp, 4\n"
		   "\n");
	++*index;
}

Instruction *new_input(void)
{
	Instruction *inc = _new_instruction();

	inc->run = input_run;
	inc->print = input_print;
	return inc;
}

void input_run(Instruction *instruction, int32_t *index)
{
	mem_get_input();
	++*index;
}

void input_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; input\n");
	fprintf(o, "\t; mällu sisendi lugemise instruktsioon\n");
	fprintf(o, "\tcall mem_get_input\n");
	++*index;
}

Instruction *new_write(int32_t what)
{
	Instruction *inc = _new_instruction();

	inc->write = what;
	inc->run = write_run;
	inc->print = write_print;
	return inc;
}

void write_run(Instruction *instruction, int32_t *index)
{
	mem_set((int8_t)instruction->write);
	++*index;
}

void write_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; write\n");
	fprintf(o, "\t ; mällu kirjutamise instruktsioon\n");
	fprintf(o, "\tpush dword %d\n", instruction->write);
	fprintf(o, "\tcall mem_set\n");
	fprintf(o, "\t; magasini tühjendamine\n\tadd esp, 4\n");
	++*index;
}

Instruction *new_begin_loop(void)
{
	Instruction *inc = _new_instruction();

	// algväärtustamine, pole õige
	// päris seadmine toimub bf_interpretator.c parse() ']' instruktsiooni all
	inc->loop_forward_index = -1;
	inc->run = begin_loop_run;
	inc->print = begin_loop_print;

	return inc;
}

void begin_loop_run(Instruction *instruction, int32_t *index)
{
	int32_t current_value = mem_get();

	if (instruction->loop_forward_index < 0) {
		fprintf(stderr, "Tsükli lõpp puudub\n");
		exit(EXIT_FAILURE);
		return;
	}

	/* Lõppu hüppamine */
	if (current_value == 0) {
		*index = instruction->loop_forward_index + 1;
		return;
	}
	++*index;
}

void begin_loop_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; begin_loop\n");
	fprintf(o, "\t ; tsükli alustamise instruktsioon\n");
	fprintf(o, "\tTSYK_%d:\n", *index);
	fprintf(o, "\t; hangi praegune mälupesa väärtus\n");
	fprintf(o, "\tcall mem_get\n");
	fprintf(o, "\t; võrdle 0iga, kui on võrdne, hüppa\n");
	fprintf(o, "\tcmp eax,0\n");
	fprintf(o, "\tje TSYK_%d\n", instruction->loop_forward_index);
	fprintf(o, "\t; muidu lase edasi lih\n");
	++*index;
}

Instruction *new_end_loop(int32_t loop_back_index)
{
	Instruction *inc = _new_instruction();

	inc->loop_backward_index = loop_back_index;
	inc->run = end_loop_run;
	inc->print = end_loop_print;

	return inc;
}

void end_loop_run(Instruction *instruction, int32_t *index)
{
	int32_t current_value = mem_get();

	if (current_value == 0) {
		++*index;
		return;
	}
	*index = instruction->loop_backward_index;
}

void end_loop_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; end_loop\n");
	fprintf(o, "\t; tsükli lõpetamise instruktsioon\n");
	fprintf(o, "\tTSYK_%d:\n", *index);
	fprintf(o, "\t; hangi praegune mälupesa väärtus\n");
	fprintf(o, "\tcall mem_get\n");
	fprintf(o, "\t; võrdle 0iga, kui on suurem, hüppa\n");
	fprintf(o, "\tcmp eax,0\n");
	fprintf(o, "\tjg TSYK_%d\n", instruction->loop_backward_index);
	fprintf(o, "\t; muidu lase edasi lih\n");
	++*index;
}

Instruction *new_output(void)
{
	Instruction *inc = _new_instruction();

	inc->run = output_run;
	inc->print = output_print;

	return inc;
}

void output_run(Instruction *instruction, int32_t *index)
{
	int32_t current = mem_get();
	/*  if (current == EOF) {
		 *index = -1;
		 return;
	 } */
	putchar((char)current);
	++*index;
}

void output_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; output\n");
	fprintf(o, "\t; mälupesa väärtuse väljastamise instruktsioon\n");
	fprintf(o, "\t; hangi praegune mälupesa väärtus\n");
	fprintf(o, "\tcall mem_get\n");
	fprintf(o, "\tpush eax\n");
	fprintf(o, "\t; väljasta\n");
	fprintf(o, "\tcall putchar\n");
	fprintf(o, "\t; tühjenda magasin\n\tadd esp, 4\n");
	++*index;
}

Instruction *new_print(void)
{
	Instruction *inc = _new_instruction();

	inc->run = print_run;
	inc->print = print_print;

	return inc;
}

void print_run(Instruction *instruction, int32_t *index)
{
	mem_print_debug();
	++*index;
}

void print_print(Instruction *instruction, int32_t *index)
{
	FILE *o = get_print_output();
	fprintf(o, "\t;;; print\n");
	fprintf(o, "\t; silumisinfo väljastamise instruktsiooon\n");
	fprintf(o, "\tcall mem_print_debug\n");
	++*index;
}

void destroy_instruction(Instruction **pointer_to_instruction)
{
	free(*pointer_to_instruction);
	*pointer_to_instruction = NULL;
}

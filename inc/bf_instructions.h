#ifndef __BF_INSTRUCTIONS_H__
#define __BF_INSTRUCTIONS_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Selle struktuuriga esitame kõiki Brainfuck instruktsioone koos
   lisainfoga, mis võimaldavad optimeeringuid.
*/
typedef struct bf_instruction {
	/* Instruktsiooni väljakutse funktsioon. Esimene parameeter on viit
       instruktsiooni objektile teine on viit instruktsiooni indeksile, mida
       antud funktsioon peab kas suurendama või muutma (näiteks tsüklites).
    */
	void (*run)(struct bf_instruction *instruction, int32_t *index);

	// printimise funktsioon, mis instruktsiooni teksti kujul väljastab
	void (*print)(struct bf_instruction *instruction, int32_t *index);

	/* Implementatsiooni spetsiifilised väärtused. */
	union {
		/* Suurenda liitmisel väärtust selle võrra (võib olla ka negatiivne). */
		int32_t increment;

		/* Mitu kohta tuleb mälu asukohta liigutada (võib olla ka negatiivne).*/
		int32_t number_of_positions;

		/* Indeks kuhu hüpata tsükli alguses kui getMem() == 0. */
		int32_t loop_forward_index;

		/* Indeks kuhu hüpata tagasi tsükli lõpus kui getMem() != 0. */
		int32_t loop_backward_index;

		/* Kirjuta mällu see väärtus */
		int32_t write;
	};

} Instruction;

Instruction *new_increment(int32_t increment);
void increment_run(Instruction *instruction, int32_t *index);
void increment_print(Instruction *instruction, int32_t *index);

Instruction *new_move(int32_t move);
void move_run(Instruction *instruction, int32_t *index);
void move_print(Instruction *instruction, int32_t *index);

Instruction *new_input(void);
void input_run(Instruction *instruction, int32_t *index);
void input_print(Instruction *instruction, int32_t *index);

Instruction *new_write(int32_t what);
void write_run(Instruction *instruction, int32_t *index);
void write_print(Instruction *instruction, int32_t *index);

Instruction *new_begin_loop(void);
void begin_loop_run(Instruction *instruction, int32_t *index);
void begin_loop_print(Instruction *instruction, int32_t *index);

Instruction *new_end_loop(int32_t loop_back_index);
void end_loop_run(Instruction *instruction, int32_t *index);
void end_loop_print(Instruction *instruction, int32_t *index);

Instruction *new_output(void);
void output_run(Instruction *instruction, int32_t *index);
void output_print(Instruction *instruction, int32_t *index);

Instruction *new_print(void);
void print_run(Instruction *instruction, int32_t *index);
void print_print(Instruction *instruction, int32_t *index);

void destroy_instruction(Instruction **pointer_to_instruction);

#endif // __BF_INSTRUCTIONS_H__

#ifndef __BF_INTERPRETATOR_H__
#define __BF_INTERPRETATOR_H__

#ifdef DEBUG
#define PRINT_PARAMS "'%c' (byte = %d)\n", c, c
#define DB_MESSAGE(MSG) printf MSG
#else
#define PRINT_PARAMS "%c", c
#define DB_MESSAGE(MSG)

#endif // DEBUG

#include "bf_instructions.h"

#include <stdint.h>
#include <stdbool.h>

void interpret(const char *program, bool do_print_asm);
void parse(Instruction **inst_arr, const char *program);
void run(Instruction **instructions, int32_t len_instructs);
void print_asm(Instruction **instructions, int32_t len_instructs);

#endif // __BF_INTERPRETATOR_H__

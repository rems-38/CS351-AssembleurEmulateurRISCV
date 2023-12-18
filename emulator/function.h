#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t *readFile(FILE *finput);
void execute_instr(Processor *cpu, uint32_t instr);
void emulate_prog(Processor *cpu, uint32_t *instr);
void write_state(FILE *foutput, Processor *cpu);
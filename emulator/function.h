#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void init(Processor *cpu);
void readFile(FILE *finput, Processor *cpu);
Instruction decode_instr(uint32_t instr);
void execute_instr(Processor *cpu, Instruction instr, int *incr_pc);
void emulate_prog(Processor *cpu);
void write_state(FILE *foutput, Processor *cpu);
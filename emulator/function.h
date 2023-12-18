#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t *readFile(FILE *finput);
void execute(uint32_t *instr, uint32_t registres[33]);
void write_state(FILE *foutput, uint32_t registres[33]);
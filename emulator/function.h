#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t *readFile(FILE *finput);
void write_state(FILE *foutput, uint32_t registres[33]);
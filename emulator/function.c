#include "function.h"

uint32_t *readFile(FILE *finput) {
    int count = 0;
    unsigned int buf = 0;
    while (fscanf(finput, "%08x", &buf) != EOF) {
        count++;
    }

    fseek(finput, 0, SEEK_SET);
    uint32_t *instr = malloc(count * sizeof(uint32_t));
    uint32_t line = 0;
    count = 0;
    while (fscanf(finput, "%08x", &line) != EOF) {
        *(instr + count) = line;
        count++;
    }

    return instr;
}


void write_state(FILE *foutput, uint32_t registres[33]) {
    for (int i = 0; i < 32; i++) {
        fprintf(foutput, "x%.2d: %d\n", i, registres[i]);
    }
    fprintf(foutput, "pc: %d\n", registres[32]);
}

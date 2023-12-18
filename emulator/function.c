#include "function.h"

uint32_t *readFile(FILE *finput) {
    int count = 0;
    unsigned int buf = 0;
    while (fscanf(finput, "%08x", &buf) != EOF) {
        count++;
    }

    fseek(finput, 0, SEEK_SET);
    uint32_t *instr = calloc(count + 1, sizeof(uint32_t)); // Calloc sets all values to 0
    if (instr == NULL) {
        printf("Error: could not allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        if (fscanf(finput, "%08x", &instr[i]) == EOF) { break; }
    }

    return instr;
}

void execute(uint32_t *instr, uint32_t registres[33]) {
    
}


void write_state(FILE *foutput, uint32_t registres[33]) {
    for (int i = 0; i < 32; i++) {
        fprintf(foutput, "x%.2d: %d\n", i, registres[i]);
    }
    fprintf(foutput, "pc: %d\n", registres[32]);
}

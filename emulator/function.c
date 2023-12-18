#include "const.h"
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

void execute_instr(Processor *cpu, uint32_t instr) {
    return;
}

void emulate_prog(Processor *cpu, uint32_t *instr) {
    while (instr[cpu->pc / 4] != 0) {
        execute_instr(cpu, instr[cpu->pc / 4]);
        cpu->pc += 4;
    }
}

void write_state(FILE *foutput, Processor *cpu) {
    for (int i = 0; i < 32; i++) {
        fprintf(foutput, "x%.2d: %d\n", i, cpu->reg[i]);
    }
    fprintf(foutput, "pc: %d\n", cpu->pc);
}

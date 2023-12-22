#include "const.h"
#include "function.h"

void init(Processor *cpu) {
    for (int i = 0; i < 32; i++) {
        cpu->reg[i] = 0;
    }
    cpu->reg[2] = 16384/4;
    cpu->pc = 0;

    cpu->size = 16384;
    for (int i = 0; i < cpu->size/4; i++) {
        cpu->mem[i] = 0;
    }
}

void readFile(FILE *finput, Processor *cpu) {
    int count = 0;
    unsigned int buf = 0;
    while (fscanf(finput, "%08x", &buf) != EOF) {
        count++;
    }

    fseek(finput, 0, SEEK_SET);
    for (int i = 0; i < count; i++) {
        if (fscanf(finput, "%08x", &(cpu->mem[i])) == EOF) { break; }
    }
}

Instruction decode_instr(uint32_t instr) {
    Instruction ret = {0, 0, 0, 0, 0};



    return ret;
}

void execute_instr(Processor *cpu, Instruction instr) {
    return;
}

void emulate_prog(Processor *cpu) {
    while(cpu->mem[cpu->pc] != 0) {
        execute_instr(cpu, decode_instr(cpu->mem[cpu->pc]));
        cpu->pc++;
    }
}

void write_state(FILE *foutput, Processor *cpu) {
    for (int i = 0; i < 32; i++) {
        fprintf(foutput, "x%.2d: %d\n", i, cpu->reg[i]);
    }
    fprintf(foutput, "pc: %d\n", cpu->pc);
}

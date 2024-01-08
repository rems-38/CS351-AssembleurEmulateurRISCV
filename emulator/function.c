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

Instruction decode_instr(uint32_t word) {
    Instruction instr = {0, 0, 0, 0, 0};
    int opcode = word & 0x7f;
    if (opcode == 0x33 || opcode == 0x13) { // add, sub, addi
        instr.pattern = 0;
        instr.result = (word >> 7) & 0x1f;
        instr.ope1 = (word >> 15) & 0x1f;
        instr.ope2 = (word >> 20) & 0x1f;
        instr.settings = 1;
        if (opcode == 0x33 && ((word >> 25) & 0x7f) == 0x20) instr.settings = -1; // for sub
    }
    else if (opcode == 0x03 || opcode == 0x23 ) { // ld, sd
        instr.pattern = 1;
        instr.result = (word >> 7) & 0x1f;
        instr.ope1 = (word >> 15) & 0x1f;
        instr.ope2;
        if (opcode == 0x03) instr.settings = 0;
        else instr.settings = 1;
    }
    else if (opcode == 0x63) { // beq, bne, blt, bge
        instr.pattern = 2;
        if (((word >> 25) & 0x7f) == 0x00) instr.settings = 0;
        else if (((word >> 25) & 0x7f) == 0x01) instr.settings = 1;
        else if (((word >> 25) & 0x7f) == 0x04) instr.settings = 2;
        else if (((word >> 25) & 0x7f) == 0x05) instr.settings = 3;
        instr.ope2;
    } else if (opcode == 0x6f) { // jal
        instr.pattern = 3;
        instr.result = (word >> 7) & 0x1f;
        instr.ope2;
    }

    // printf("pattern: %d\n", instr.pattern);
    // printf("result: %d\n", instr.result);
    // printf("ope1: %d\n", instr.ope1);
    // printf("ope2: %d\n", instr.ope2);
    // printf("settings: %d\n\n", instr.settings);

    return instr;
}

void execute_instr(Processor *cpu, Instruction instr) {
    if (instr.pattern == 0) cpu->reg[instr.result] = cpu->reg[instr.ope1] + instr.settings * cpu->reg[instr.ope2];
    else if (instr.pattern == 1) {
        if (instr.settings == 0) cpu->reg[instr.result] = cpu->mem[cpu->reg[instr.ope1] + cpu->reg[instr.ope2]];
        else if (instr.settings == 1) cpu->mem[cpu->reg[instr.ope1] + cpu->reg[instr.ope2]] = cpu->reg[instr.result];
    }
    else if (instr.pattern == 2) {
        if (instr.settings == 0) if (cpu->reg[instr.ope1] == cpu->reg[instr.ope2]) cpu->pc += instr.result;
        else if (instr.settings == 1) if (cpu->reg[instr.ope1] != cpu->reg[instr.ope2]) cpu->pc += instr.result;
        else if (instr.settings == 2) if (cpu->reg[instr.ope1] < cpu->reg[instr.ope2]) cpu->pc += instr.result;
        else if (instr.settings == 3) if (cpu->reg[instr.ope1] >= cpu->reg[instr.ope2]) cpu->pc += instr.result;
    }
    else if (instr.pattern == 3) {
        cpu->reg[instr.result] = cpu->pc + 1;
        cpu->pc += cpu->reg[instr.ope2]; // peut etre à diviser par 4 (à voir)
    }
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

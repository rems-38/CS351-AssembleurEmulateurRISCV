#include "const.h"
#include "function.h"

void init(Processor *cpu) {
    for (int i = 0; i < 32; i++) {
        cpu->reg[i] = 0;
    }
    cpu->reg[2] = 16384;
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
    int opcode = word & 0x7f; // 0x7f -> 0111 1111 donc 7 bits
    if (opcode == 0x33 || opcode == 0x13) { // add, sub, addi
        instr.pattern = 0;
        instr.result = (word >> 7) & 0x1f; // 0x1f -> 0001 1111 donc 5 bits
        instr.ope1 = (word >> 15) & 0x1f;
        instr.ope2 = (word >> 20) & 0x1f;
        instr.settings = 1;
        if (opcode == 0x33 && ((word >> 25) & 0x7f) == 0x20) instr.settings = -1; // for sub
        else if (opcode == 0x13) { // pour addi
            instr.ope2 = (word >> 20) & 0xfff;
            if (word >> 31) instr.ope2 -= 4096; // imm (negative value)
            instr.settings = 2;
        }
    }
    else if (opcode == 0x03 || opcode == 0x23 ) { // ld, sd
        instr.pattern = 1;
        instr.ope1 = (word >> 15) & 0x1f;
        if (opcode == 0x03) {
            instr.result = (word >> 7) & 0x1f;
            instr.ope2 = (word >> 20) & 0xfff;
            if (word >> 31) instr.ope2 -= 4096;
            instr.settings = 0; // ld
        }
        else {
            instr.result = (word >> 20) & 0x1f;
            instr.ope2 = ((word >> 7) & 0x1f) + 32*((word >> 25) & 0x7f);
            if (word >> 31) instr.ope2 -= 4096;
            instr.settings = 1; // sd
        } 
    }
    else if (opcode == 0x63) { // beq, bne, blt, bge
        instr.pattern = 2;
        instr.ope1 = (word >> 15) & 0x1f;
        instr.ope2 = (word >> 20) & 0x1f;
        if (((word >> 12) & 0x7) == 0x00) instr.settings = 0; // beq
        else if (((word >> 12) & 0x7) == 0x01) instr.settings = 1; // bne
        else if (((word >> 12) & 0x7) == 0x04) instr.settings = 2; // blt
        else if (((word >> 12) & 0x7) == 0x05) instr.settings = 3; // bge
        instr.result = 2*((word >> 8) & 0xf) + 32*((word >> 25) & 0x3f) + 2048*((word >> 7) & 0x1) + 4096*(word >> 31); // imm
        if (word >> 31) instr.result -= 8192; // imm (negative value)
    } else if (opcode == 0x6f) { // jal
        instr.pattern = 3;
        instr.result = (word >> 7) & 0x1f;
        instr.ope2 = 2*((word >> 21) & 0x3ff) + 2048*((word >> 20) & 0x1) + 4096*((word >> 12) & 0xff) + 1048576*(word >> 31); // imm
        if (word >> 31) instr.ope2 -= 2097152; // imm (negative value)
    }

    return instr;
}

void execute_instr(Processor *cpu, Instruction instr, int *incr_pc) {
    *incr_pc = 1;
    if (instr.pattern == 0 && instr.result != 0) {
        if (instr.settings == 2) cpu->reg[instr.result] = cpu->reg[instr.ope1] + instr.ope2;
        else cpu->reg[instr.result] = cpu->reg[instr.ope1] + instr.settings * cpu->reg[instr.ope2];
    }
    else if (instr.pattern == 1) {
        if (instr.settings == 0 && instr.result != 0) cpu->reg[instr.result] = cpu->mem[(cpu->reg[instr.ope1] + instr.ope2) / 4]; 
        else if (instr.settings == 1) cpu->mem[(cpu->reg[instr.ope1] + instr.ope2) / 4] = cpu->reg[instr.result];
    }
    else if (instr.pattern == 2) {
        if (instr.settings == 0) { if (cpu->reg[instr.ope1] == cpu->reg[instr.ope2]) { cpu->pc += instr.result / 4; *incr_pc = 0; } }
        else if (instr.settings == 1) { if (cpu->reg[instr.ope1] != cpu->reg[instr.ope2]) { cpu->pc += instr.result / 4; *incr_pc = 0; } }
        else if (instr.settings == 2) { if (cpu->reg[instr.ope1] < cpu->reg[instr.ope2]) { cpu->pc += instr.result / 4; *incr_pc = 0; } }
        else if (instr.settings == 3) { if (cpu->reg[instr.ope1] >= cpu->reg[instr.ope2]) { cpu->pc += instr.result / 4; *incr_pc = 0; } }
    }
    else if (instr.pattern == 3) {
        *incr_pc = 0;
        if (instr.result != 0) cpu->reg[instr.result] = (cpu->pc + 1) * 4; // jal x0, imm (on ne save rien dans x0)
        cpu->pc += (instr.ope2 / 4);
    }
}

void emulate_prog(Processor *cpu) {
    int incr_pc = 1;
    while(cpu->mem[cpu->pc] != 0) {
        execute_instr(cpu, decode_instr(cpu->mem[cpu->pc]), &incr_pc);
        if (incr_pc) cpu->pc++;
    }
}

void write_state(FILE *foutput, Processor *cpu) {
    for (int i = 0; i < 32; i++) { 
        fprintf(foutput, "x%d: %ld\n", i, cpu->reg[i]);
    }
}


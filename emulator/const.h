#include <stdint.h>

typedef struct {
    uint32_t reg[32];
    uint32_t mem[4096];
    uint32_t pc;
    int size;
} Processor;

typedef struct {
    int pattern;
    int result;
    int ope1;
    int ope2;
    int settings;
} Instruction;

/*
Explication Instruction (en gros) :
    Pattern : 0, 1, 2 ou 3 (une addition, avec pile, test/PC, jump)
    Result : rd en général
    Ope1 : rs1 en général
    Ope2 : rs2 ou imm
    Settings :
        - add: 1
        - sub: -1
        - addi: 2
     
        - (==, !=, <, >=): 0, 1, 2, 3
     
        - ld: 0
        - sd: 1
*/
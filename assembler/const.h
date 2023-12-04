char *types[13][5] = {{"add", "0", "0110011", "000", "0000000"},
                    {"sub", "0", "0110011", "000", "0100000"},
                    {"addi", "1", "0010011", "000", NULL},
                    {"ld", "1", "0000011", "011", NULL},
                    {"sd", "2", "0100011", "011", NULL},
                    {"beq", "3", "1100011", "000", NULL},
                    {"bne", "3", "1100011", "001", NULL},
                    {"blt", "3", "1100011", "100", NULL},
                    {"bge", "3", "1100011", "101", NULL},
                    {"jal", "4", "1101111", NULL, NULL},
                    {"j", "P", "jal", NULL, NULL},
                    {"li", "P", "addi", NULL, NULL},
                    {"mv", "P", "addi", NULL, NULL}};

char *registres[32] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2",
                        "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9",
                        "s10", "s11", "t3", "t4", "t5", "t6"};

int instr_format[5][8][3] = {
    {{0, -1, -1}, {7, 5, -1}, {15, 5, -1}, {20, 5, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}},
    {{1, 1, -1}, {7, 5, -1}, {15, 5, -1}, {20, 12, 0}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}},
    {{1, 2, -1}, {20, 5, -1}, {15, 5, -1}, {-1, 12, -1}, {7, 5, 0}, {25, 7, 5}, {-1, -1, -1}, {-1, -1, -1}},
    {{1, 4, -1}, {15, 5, -1}, {20, 5, -1}, {-1, 12, -1}, {7, 1, 11}, {8, 4, 1}, {25, 6, 5}, {31, 1, 11}},
    {{1, 4, -1}, {7, 5, -1}, {-1, 20, -1}, {-1, -1, -1}, {12, 8, 12}, {20, 1, 11}, {21, 10, 1}, {31, 1, 19}}
};

/*
Explication instr_format (R:0, I:1, S:2, B:3, J:4)
{
    {si imm ou pas, nombre de "partitionnement", -1},
    {start, length, -1}, 1er paramètre
    {start, length, -1}, 2ème paramètre (sauf J où c'est settings im)
    {start, length, -1}, 3ème paramètre ou settings imm (length tot) + start si en une partie
    {start, length, offset}, partie 1 de imm
    {start, length, offset}, partie 2 de imm
    {start, length, offset}, partie 3 de imm
    {start, length, offset} partie 4 de imm

    Si -1 => pas nécessaire pour ce bloc-ci
*/

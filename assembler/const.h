char *types[10][5] = {{"add", "R", "0110011", "000", "0000000"},
                    {"sub", "R", "0110011", "000", "0100000"},
                    {"addi", "I", "0010011", "000", NULL},
                    {"ld", "I", "0000011", "011", NULL},
                    {"sd", "S", "0100011", "011", NULL},
                    {"beq", "B", "1100011", "000", NULL},
                    {"bne", "B", "1100011", "001", NULL},
                    {"blt", "B", "1100011", "100", NULL},
                    {"bge", "B", "1100011", "101", NULL},
                    {"jal", "J", "1101111", NULL, NULL}};

char *registres[32] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2",
                        "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9"
                        "s10", "s11", "t3", "t4", "t5", "t6"};

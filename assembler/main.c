#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "function.h"
#include "const.h"

int main(int argc, char **argv) {
    /* ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT> */
    if(argc != 3) {
        printf("usage: ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT>\n");
        return 1;
    }

    char *asm_input_file = argv[1];
    char *hex_output_file = argv[2];

    FILE *finput = fopen(asm_input_file, "r");
    FILE *foutput = fopen(hex_output_file, "w");

    if(finput == NULL) {
        printf("Error: could not open file %s\n", asm_input_file);
        return 1;
    }

    size_t ch = 0;
    size_t bufsize = 32;
    char *line = (char *)malloc(bufsize * sizeof(char));
    char **tab = NULL;
    char **infos = NULL;
    uint32_t output = 0;

    while ((ch = getline(&line, &bufsize, finput)) != (size_t)EOF) {
        if (*line != '#' && *line != ' ' && *line != '\n') {
            tab = parse_string(line);
            infos = get_infos(tab[0], types);
            output = 0;

            // Ecriture de l'opcode (toujours en 1er)
            write_output(infos[1], &output, 0, 7);

            // Func3 (de partout sauf pour les J)
            if (strcmp(infos[0], "J") != 0) write_output(infos[2], &output, 12, 3);
            
            if (strcmp(infos[0], "R") == 0) {
                write_output(infos[3], &output, 25, 7); // func7
                write_output(to_bin(find_registrer(tab[1], registres), 5), &output, 7, 5); // rd
                write_output(to_bin(find_registrer(tab[2], registres), 5), &output, 15, 5); // rs1
                write_output(to_bin(find_registrer(tab[3], registres), 5), &output, 20, 5); // rs2
            } else if (strcmp(infos[0], "I") == 0) {
                write_output(to_bin(find_registrer(tab[1], registres), 5), &output, 7, 5); // rd
                write_output(to_bin(find_registrer(tab[2], registres), 5), &output, 15, 5); // rs1
                write_output(to_bin(atoi(tab[3]), 12), &output, 20, 12); // imm
            } else if (strcmp(infos[0], "S") == 0) {
                printf("S\n");
            } else if (strcmp(infos[0], "B") == 0) {
                printf("B\n");
            } else if (strcmp(infos[0], "J") == 0) {
                printf("J\n");
            } else {
                printf("Unknown instruction : %s\n", infos[0]);
            }

            // Libération de la mémoire
            for (int i = 0; i < 4; i++) free(infos[i]);
            free(infos);
            // for (int i = 0; i < sizeof(tab); i++) free(tab[i]);
            free(tab);

            // Ecrire dans le fichier
            fprintf(foutput, "%08x\n", output);
        }
    }

    fclose(finput);
    fclose(foutput);

    return 0;
}

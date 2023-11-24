#include <stdio.h>
#include <stdlib.h>
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

    while ((ch = getline(&line, &bufsize, finput)) != (size_t)EOF) {
        if (*line != '#' && *line != ' ' && *line != '\n') {
            tab = parse_string(line);
            char **infos = get_infos(tab[0], types);

            for (int i = 0; i < 4; i++) printf("%s ", infos[i]);
            printf("\n");

            // Libération de la mémoire
            for (int i = 0; i < 4; i++) {
                free(infos[i]);
            }
            free(infos);
        }
    }

    fclose(finput);
    fclose(foutput);

    return 0;
}

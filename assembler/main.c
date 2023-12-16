#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "function.h"
#include "const.h"

int main(int argc, char **argv) {
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
    int nb_word = 0;

    while ((ch = getline(&line, &bufsize, finput)) != (size_t)EOF) {
        if (*line != '#' && *line != ' ' && *line != '\n') {
            tab = parse_string(line, &nb_word);
            infos = get_infos(tab[0], types);
            if (infos[0] == NULL) { fprintf(foutput, "%.8d\n", 0); }
            else {
                output = 0;
                if (strcmp(infos[0], "P") == 0) pseudo_replace(tab, infos, types, &nb_word);
                instr_parsing(tab, infos, &output, registres, instr_format);

                // Libération de la mémoire
                for (int i = 0; i < 4; i++) free(infos[i]);
                free(infos);
                for (int i = 0; i < nb_word; i++) free(tab[i]);
                free(tab);
                nb_word = 0;

                fprintf(foutput, "%08x\n", output);
            }
        }
    }

    fclose(finput);
    fclose(foutput);

    return 0;
}

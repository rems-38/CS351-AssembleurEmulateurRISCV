#include "function.h"
#include <string.h>

int main(int argc, char **argv)
{
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
        if (*line != '#') {
            tab = parse_string(line);
            
            for(size_t i = 0; i < strlen(*tab); i++) {
                printf("%s\n", tab[i]);
            }
        }
    }


    fclose(finput);
    fclose(foutput);

    return 0;
}

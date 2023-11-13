#include <stdio.h>

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


    fclose(finput);
    fclose(foutput);

    return 0;
}

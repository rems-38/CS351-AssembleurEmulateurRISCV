#include <stdio.h>
#include "const.h"
#include "function.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Usage: ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT>\n");
        printf("error: invalid command\n");
        return 1;
    }

    char *hex_input_file = argv[1];
    char *emu_output_file = argv[2];
    FILE *finput = fopen(hex_input_file, "r");
    FILE *foutput = fopen(emu_output_file, "w");

    if(finput == NULL) {
        printf("Error: could not open file %s\n", hex_input_file);
        return 1;
    }

    Processor cpu;
    
    uint32_t *instr = readFile(finput);
    emulate_prog(&cpu, instr);

    free(instr);

    write_state(foutput, &cpu);
    fclose(finput);
    fclose(foutput);

    return 0;
}

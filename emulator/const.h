#include <stdint.h>

typedef struct {
    uint32_t reg[32];
    uint32_t mem[4096];
    uint32_t pc;
    int size;
} Processor;

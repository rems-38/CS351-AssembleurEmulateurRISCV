#include <stdio.h>
#include <stdlib.h>

char **parse_string(char *str);
char **get_infos(char *str, char *types[13][5]);
void write_output(char *data, uint32_t *output, int start, int length);
int find_registrer(char *str, char *registres[32]);
char *to_bin(int n, int length);
char *flip(char *str, int length);
void print_bin(uint32_t n);
void pseudo_replace(char **tab, char **infos, char *types[13][5]);
void instr_parsing(char **tab, char **infos, uint32_t *output, char *registres[32]);
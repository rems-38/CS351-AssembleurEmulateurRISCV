#include <stdio.h>
#include <stdlib.h>

char **parse_string(char *str, int *nb_word);
char **get_infos(char *str, char *types[13][5]);
void write_output(char *data, uint32_t *output, int start, int length);
int find_registrer(char *str, char *registres[32]);
char *to_bin(int n, int length);
char *flip(char *str, int length);
void pseudo_replace(char **tab, char **infos, char *types[13][5], int *nb_word);
void error(char *str, int *output);
void instr_parsing(char **tab, char **infos, uint32_t *output, char *registres[32], int instr_format[5][8][3]);

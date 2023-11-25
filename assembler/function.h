#include <stdio.h>
#include <stdlib.h>

char **parse_string(char *str);
char **get_infos(char *str, char *types[10][5]);
void write_output(char *data, uint32_t *output, int start, int length);
#include <string.h>
#include <stdint.h>
#include "function.h"

char **parse_string(char *str) {
    // Comptage du nombre d'élements
    int nb_word = 0;
    for(int i = 0; *(str + i) != '\0'; i++) {
        if(*(str + i) == ' ' || *(str + i) == ',') {
            if(*(str + i + 1) != ' ') {
                i++;
            }
            nb_word++;
        }
    }

    // Allocation de la mémoire
    char **tab = malloc((nb_word + 1) * sizeof(char *));
    for(int i = 0; i < nb_word + 1; i++) {
        tab[i] = malloc(10 * sizeof(char));
    }

    // Remplissage du tableau
    int j = 0;
    int k = 0;
    for(int i = 0; *(str + i) != '\0'; i++) {
        if(*(str + i) == ' ' || *(str + i) == ',' || *(str + i) == '\n') {
            *(tab[j] + k) = '\0';
            if (*(str + i + 1) == ' ') i++;
            j++;
            k = 0;
        } else {
            *(tab[j] + k) = *(str + i);
            k++;
        }
    }

    return tab;
}

char **get_infos(char *str, char *types[10][5]) {
    char **infos = malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++) {
        infos[i] = malloc(7 * sizeof(char)); // Au plus une string de 7 bits (opcode)
    }

    for (int i = 0; i < 10; i++) {
        if (strcmp(str, types[i][0]) == 0) {
            strcpy(infos[0], types[i][1]);
            strcpy(infos[1], types[i][2]);
            if (types[i][3] != NULL) strcpy(infos[2], types[i][3]);
            else infos[2] = NULL; // NULL est pas un char* donc strcpy seg fault 
            if (types[i][4] != NULL) strcpy(infos[3], types[i][4]);
            else infos[3] = NULL;

            break;
        }
    }

    return infos;
} 

void write_output(char *data, uint32_t *output, int start, int length) {
    for (int i = start, j = 0; i < start+length; i++, j++) {
        // printf("%d %d %d %d\n", (data[j] & 1), i, (data[j] & 1) << i, *output);
        *output |= ((data[(length-1)-j] & 1) << i);
    }
}

int find_registrer(char *str, char *registres[32]) {
    for (int i = 0; i < 32; i++) {
        if (strcmp(str, registres[i]) == 0) {
            return i;
        }
    }
    return 0;
}

char *to_bin(int n, int length) {
    char *str = malloc(length+1 * sizeof(char));
    str[length] = '\0';

    for (int i = length-1; i >= 0; --i, n >>= 1) {
        str[i] = (n & 1) + '0';
    }

    return str;
}
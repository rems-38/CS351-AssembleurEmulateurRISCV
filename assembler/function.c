#include <string.h>
#include "function.h"

char **parse_string(char *str) {
    int nb_word = 0;
    for(int i = 0; *(str + i) != '\0'; i++) {
        if(*(str + i) == ' ' || *(str + i) == ',') {
            if(*(str + i + 1) != ' ') {
                i++;
            }
            nb_word++;
        }
    }

    char **tab = malloc((nb_word + 1) * sizeof(char *));
    for(int i = 0; i < nb_word + 1; i++) {
        tab[i] = malloc(10 * sizeof(char));
    }

    int j = 0;
    int k = 0;
    for(int i = 0; *(str + i) != '\0'; i++) {
        if(*(str + i) == ' ' || *(str + i) == ',') {
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
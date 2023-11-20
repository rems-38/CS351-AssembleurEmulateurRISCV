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

#include <string.h>
#include <stdint.h>
#include "function.h"

char *remove_spaces(char *str) {
    char *new_str = malloc(strlen(str) * sizeof(char));
    int j = 0;
    int first_word = 1;
    int start_of_line = 1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            if (start_of_line) continue;
            if (first_word) {
                new_str[j] = str[i];
                j++;
                first_word = 0;
            }
        } else if (str[i] == '#') {
            new_str[j] = '\n';
            j++;
            break;
        } else {
            new_str[j] = str[i];
            j++;
            if (start_of_line) start_of_line = 0;
        }
    }
    new_str[j] = '\0';
    return new_str;
}

char **parse_string(char *str, int *nb_word) {
    *nb_word = 5;

    // Suppression des espaces
    char *nstr = remove_spaces(str);

    // Allocation de la mémoire
    char **tab = malloc(*nb_word * sizeof(char *));
    for(int i = 0; i < *nb_word; i++) {
        tab[i] = malloc(10 * sizeof(char)); // On suppose que 10 est suffisant (supposition max : 7 => imm value max 2^20 => 1048576)
    }

    // Remplissage du tableau
    int j = 0, k = 0;
    int inversion = 0;
    for(int i = 0; *(nstr + i) != '\0'; i++) {
        if(*(nstr + i) == ' ' || *(nstr + i) == ',' || *(nstr + i) == '\n' || *(nstr + i) == '(') {
            if (*(nstr + i) == '(') inversion = 1; // pour après
            *(tab[j] + k) = '\0';
            if (*(nstr + i + 1) == ' ') i++; // pour ", "
            j++;
            k = 0;
        } else if (*(nstr + i) == ')') {
            *(tab[j] + k) = '\0';
            j++;
            k = 0;
        } else {
            *(tab[j] + k) = *(nstr + i);
            k++;
        }
    }

    // Inversion des deux derniers élements (si on a vu des paranthèses, c'est un cas particulier (offset...))
    if (inversion) {
        char *tmp = tab[j-2]; // On prend j-2 car j=5 à la fin et dernier = tab[3] (pas toujours mais on verse quand pour du 4...)
        tab[j-2] = tab[j-3];
        tab[j-3] = tmp;
    }

    // Libération de la mémoire
    free(nstr);

    return tab;
}

char **get_infos(char *str, char *types[13][5]) {
    char **infos = malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++) {
        infos[i] = malloc(7 * sizeof(char)); // Au plus une string de 7 bits (opcode)
    }

    int iGetIt = 0;
    for (int i = 0; i < 13; i++) {
        if (strcmp(str, types[i][0]) == 0) {
            for (int j = 0; j < 4; j++) {
                if (types[i][j+1] != NULL) strcpy(infos[j], types[i][j+1]);
                else infos[j] = NULL; // NULL est pas un char* donc strcpy seg fault 
            }
            iGetIt = 1;
            break;
        }
    }
    if (!iGetIt) {
        printf("Error: instruction %s not found\n", str);
        infos[0] = NULL;
    }
    return infos;
} 

void write_output(char *data, uint32_t *output, int start, int length) {
    for (int i = start, j = 0; i < start+length; i++, j++) {
        *output |= ((data[(length-1)-j] & 1) << i);
    }
}

int find_registrer(char *str, char *registres[32]) {
    char *buf = malloc(3 * sizeof(char));
    int to_return = -1;
    for (int i = 0; i < 32; i++) {
        sprintf(buf, "x%d", i);
        if (strcmp(str, buf) == 0) {to_return = i; break; }
        else if (strcmp(str, registres[i]) == 0) { to_return = i; break; }
    }
    if (to_return == -1) { printf("Error: registrer %s not found\n", str); }
    free(buf);
    return to_return;
}

char *to_bin(int n, int length) {
    char *str = malloc(length+1 * sizeof(char));
    str[length] = '\0';
    if (n < 0){
        int tmp_n = 1;
        for (int i = 0; i < length; i++) tmp_n *= 2;
        n = tmp_n - abs(n); // Complément à 2 : 2^(longueur) - |n| puis on met en binaire
    }
    for (int i = length-1; i >= 0; --i, n >>= 1) str[i] = (n & 1) + '0';
    return str;
}

char *flip(char *str, int length) {
    char *new_str = malloc(length * sizeof(char));
    if (length == 1) { return str; } 
    for (int i = 0; i < length; i++) new_str[i] = str[(length-1)-i];
    return new_str;
}

void pseudo_replace(char **tab, char **infos, char *types[13][5], int *nb_word) {    
    if (strcmp(tab[0], "j") == 0) {
        strcpy(tab[2], tab[1]);
        strcpy(tab[1], "zero");
    } else if (strcmp(tab[0], "li") == 0) {
        strcpy(tab[3], tab[2]);
        strcpy(tab[2], "zero");
    } else if (strcmp(tab[0], "mv") == 0) {
        strcpy(tab[3], "0");
    }
    strcpy(tab[0], infos[1]);

    // Libération mémoire (sinon écrasement ne fonctionne pas)
    for (int i = 0; i < 4; i++) free(infos[i]);
    free(infos);
    infos = get_infos(tab[0], types); // Udpate infos data
}

void error(char *str, uint32_t *output) {
    printf("Error: %s is not a number\n", str);
    *output = 0;
}

void instr_parsing(char **tab, char **infos, uint32_t *output, char *registres[32], int instr_format[5][8][3]) {
    write_output(infos[1], output, 0, 7); // opcode (always same pos)

    if (strcmp(infos[0], "4") != 0) write_output(infos[2], output, 12, 3); // func3 (de partout sauf pour les J)
    if (strcmp(infos[0], "0") == 0) write_output(infos[3], output, 25, 7); // func7 (que pour les R)

    for (int i = 1; i <= 3; i++) {
        if (instr_format[atoi(infos[0])][0][0] && i == 3) {
            if (instr_format[atoi(infos[0])][i][0] == -1) {
                if (strcmp(infos[0], "4") == 0) i--; // pour les J, imm est dans tab[2] donc on recule
                if ((tab[i][0] >= 48 && tab[i][0] <= 57)|| tab[i][0] == '-') {
                    char *imm = flip(to_bin(atoi(tab[i]), instr_format[atoi(infos[0])][i][1]), instr_format[atoi(infos[0])][i][1]);
                    for (int j = 4; j < 4+instr_format[atoi(infos[0])][0][1]; j++) {
                        write_output(flip(imm+instr_format[atoi(infos[0])][j][2], instr_format[atoi(infos[0])][j][1]), output, instr_format[atoi(infos[0])][j][0], instr_format[atoi(infos[0])][j][1]);
                    }
                }
                else { error(tab[i], output); return; }
                if (strcmp(infos[0], "4") == 0) i++; // RAZ sinon boucle infinie                
            } else {
                if ((tab[i][0] >= 48 && tab[i][0] <= 57) || tab[i][0] == '-') {
                    write_output(to_bin(atoi(tab[i]), instr_format[atoi(infos[0])][i][1]), output, instr_format[atoi(infos[0])][i][0], instr_format[atoi(infos[0])][i][1]);
                }
                else { error(tab[i], output); return; }
            }
        } else {
            if (strcmp(infos[0], "4") == 0 && i == 2) { continue; }
            else {
                if (find_registrer(tab[i], registres) == -1) {*output = 0; return; } 
                else {
                    write_output(to_bin(find_registrer(tab[i], registres), instr_format[atoi(infos[0])][i][1]), output, instr_format[atoi(infos[0])][i][0], instr_format[atoi(infos[0])][i][1]);
                }
            }
        }
    }
}
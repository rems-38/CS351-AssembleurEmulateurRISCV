#include <string.h>
#include <stdint.h>
#include "function.h"

char **parse_string(char *str) {
    // Comptage du nombre d'élements
    // int nb_word = 0;
    // for(int i = 0; *(str + i) != '\0'; i++) {
    //     if(*(str + i) == ' ' || *(str + i) == ',' || *(str + i) == '(' || *(str + i) == ')') {
    //         if(*(str + i + 1) != ' ') {
    //             i++;
    //         }
    //         nb_word++;
    //     }
    // }

    /*
    Pour les pseudos instr si on garde le code d'au dessus ça bug => cst (temporairement)
    -> tab pas assez grand 
        -> realloc ??
    + le comptage était bizarre (1 ou 2 en trop jsp)
    */
    int nb_word = 5;

    // Allocation de la mémoire
    char **tab = malloc(nb_word * sizeof(char *));
    for(int i = 0; i < nb_word; i++) {
        tab[i] = malloc(10 * sizeof(char)); // On suppose que 10 est suffisant (supposition max : 7 => imm value max 2^20 => 1048576) <- à vérif
    }

    // Remplissage du tableau
    int j = 0;
    int k = 0;
    int inversion = 0;
    for(int i = 0; *(str + i) != '\0'; i++) {
        if(*(str + i) == ' ' || *(str + i) == ',' || *(str + i) == '\n' || *(str + i) == '(') {
            if (*(str + i) == '(') inversion = 1; // pour après
            *(tab[j] + k) = '\0';
            if (*(str + i + 1) == ' ') i++; // pour ", "
            j++;
            k = 0;
        } else if(*(str + i) == ')') {
            *(tab[j] + k) = '\0';
            j++;
            k = 0;
        } else {
            *(tab[j] + k) = *(str + i);
            k++;
        }
    }

    // Inversion des deux derniers élements (si on a vu des paranthèses, c'est un cas particulier (offset...))
    if (inversion) {
        char *tmp = tab[j-2]; // On prend j-2 car j=5 à la fin et dernier = tab[3] (pas toujours mais on verse quand pour du 4...)
        tab[j-2] = tab[j-3];
        tab[j-3] = tmp;
    }

    return tab;
}

char **get_infos(char *str, char *types[13][5]) {
    char **infos = malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++) {
        infos[i] = malloc(7 * sizeof(char)); // Au plus une string de 7 bits (opcode)
    }

    for (int i = 0; i < 13; i++) {
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

char *flip(char *str, int length) {
    char *new_str = malloc(length * sizeof(char));
    for (int i = 0; i < length; i++) {
        new_str[i] = str[(length-1)-i];
    }
    return new_str;
}

void print_bin(uint32_t n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

void pseudo_replace(char **tab, char **infos, char *types[13][5]) {
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

void instr_parsing(char **tab, char **infos, uint32_t *output, char *registres[32]) {
    /* NB :
        imm encoded 2's complement                                      ❌
        J instr: imm encode also signed offset in multiple of 2 bytes   ❌
        B & J instr: imm[0] is always 0 -> removed from encoding        ✅ 
    */
    
    // Ecriture de l'opcode (toujours en 1er)
    write_output(infos[1], output, 0, 7);

    // Func3 (de partout sauf pour les J)
    if (strcmp(infos[0], "J") != 0) write_output(infos[2], output, 12, 3);

    if (strcmp(infos[0], "R") == 0) {
        write_output(infos[3], output, 25, 7); // func7
        write_output(to_bin(find_registrer(tab[1], registres), 5), output, 7, 5); // rd
        write_output(to_bin(find_registrer(tab[2], registres), 5), output, 15, 5); // rs1
        write_output(to_bin(find_registrer(tab[3], registres), 5), output, 20, 5); // rs2
    } else if (strcmp(infos[0], "I") == 0) {
        write_output(to_bin(find_registrer(tab[1], registres), 5), output, 7, 5); // rd
        write_output(to_bin(find_registrer(tab[2], registres), 5), output, 15, 5); // rs1
        write_output(to_bin(atoi(tab[3]), 12), output, 20, 12); // imm
    } else if (strcmp(infos[0], "S") == 0) {
        write_output(to_bin(find_registrer(tab[1], registres), 5), output, 20, 5); // rs2
        write_output(to_bin(find_registrer(tab[2], registres), 5), output, 15, 5); // rs1

        char *imm = flip(to_bin(atoi(tab[3]), 12), 12);
        write_output(flip(imm, 5), output, 7, 5); // imm[4:0]
        write_output(flip(imm+5, 7), output, 25, 7); // imm[11:5]
    } else if (strcmp(infos[0], "B") == 0) {
        write_output(to_bin(find_registrer(tab[1], registres), 5), output, 15, 5); // rs1
        write_output(to_bin(find_registrer(tab[2], registres), 5), output, 20, 5); // rs2
                
        char *imm = flip(to_bin(atoi(tab[3]), 12), 12);
        write_output(imm+11, output, 7, 1); // imm[11]
        write_output(flip(imm+1, 4), output, 8, 4); // imm[4:1]
        write_output(flip(imm+5, 6), output, 25, 6); // imm[10:5]
        write_output(imm+12, output, 31, 1); // imm[12]
    } else if (strcmp(infos[0], "J") == 0) {
        write_output(to_bin(find_registrer(tab[1], registres), 5), output, 7, 5); // rd

        char *imm = flip(to_bin(atoi(tab[2]), 20), 20);
        write_output(flip(imm+12, 8), output, 12, 8); // imm[19:12]
        write_output(imm+11, output, 20, 1); // imm[11]
        write_output(flip(imm+1, 10), output, 21, 10); // imm[10:1]
        write_output(imm+20, output, 31, 1); // imm[20]
    } else {
        printf("Unknown instruction : '%s'\n", infos[0]);
    }
}
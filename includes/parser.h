#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "process.h"

int run_line (Process* proc, char* line);
void remove_blank_tokens (char** tokens, int* tokens_loc);


int run_line (Process* proc, char* line) {
    // split the line into tokens
    char** tokens = str_split(line, " ");
    int* tokens_loc = (int*)malloc(sizeof(int)*sizeof(tokens));
    int loc = 0;
    for (int i = 0; tokens[i] != NULL; i++) {
        tokens_loc[i] = loc;
        loc += strlen(tokens[i]) + 1;
    }
    remove_blank_tokens(tokens, tokens_loc);

    int c = 1;
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %i. %s\n", tokens_loc[i], tokens[i]);
        c++;
    }
    printf("\n");
    // free the tokens
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
    // advance the reader by one
    return 1;
}
void remove_blank_tokens (char** tokens, int* tokens_loc) {
    int i = 0;
    while (tokens[i] != NULL) {
        if (!tokens[i][0]) {
            for (int j = i; tokens[j] != NULL; j++) {
                tokens[j] = tokens[j+1];
                tokens_loc[j] = tokens_loc[j+1];
            }
        } else {
            i++;
        }
    }
}
#endif
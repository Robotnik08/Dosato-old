#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ast.h"
#include "../parser.h"
#include "../lexer.h"
#include "../error.h"
#include "../function.h"
#include "../variable.h"
#include "../token.h"
#include "../scope.h"
#include "../process.h"

int std_SPLIT (Process* process, const Variable* args, int argc);

int std_SPLIT (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* sep = (char*)args[1].value;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_ARRAY, NULL, 0, 1);

    char* token = strtok(str, sep);

    while (token != NULL) {
        Variable* var2 = malloc(sizeof(Variable));
        char* val = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(val, token);
        *var2 = createVariable("-lit", TYPE_STRING, val, 0, 0);
        pushArray(var, var2);
        printf ("token: %s\n", token);

        destroyVariable(var2);
        free(var2);
        token = strtok(NULL, sep);
    }

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}
#endif
#ifndef RANDOM_H
#define RANDOM_H

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

int std_SRAND (Process* process, const Variable* args, int argc);

int std_RANDINT (Process* process, const Variable* args, int argc);

int std_RAND (Process* process, const Variable* args, int argc);

int std_RANDRANGE (Process* process, const Variable* args, int argc);

int std_SRAND (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_UINT, 0});
    if (cRes) return cRes;

    srand(*(unsigned int*)args[0].value);
    
    return 0; // return code
}

int std_RANDINT (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    int* val = malloc(sizeof(int));

    *val = rand();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_RAND (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    double* val = malloc(sizeof(int));

    *val = rand() / (double)RAND_MAX;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_DOUBLE, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_RANDRANGE (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_INT, 0});
    if (cRes) return cRes;
    cRes = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes) return cRes;

    int* val = malloc(sizeof(int));

    *val = rand() % (*(int*)args[1].value - *(int*)args[0].value) + *(int*)args[0].value;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}
#endif
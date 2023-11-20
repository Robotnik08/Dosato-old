#ifndef DOSATO_TIME_H
#define DOSATO_TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../ast.h"
#include "../parser.h"
#include "../lexer.h"
#include "../error.h"
#include "../function.h"
#include "../variable.h"
#include "../token.h"
#include "../scope.h"
#include "../process.h"

int std_TIME (Process* process, const Variable* args, int argc);

int std_DATE (Process* process, const Variable* args, int argc);

int std_DATETIME (Process* process, const Variable* args, int argc);

int std_TIMESTAMP (Process* process, const Variable* args, int argc);

int std_CLOCK (Process* process, const Variable* args, int argc);

int std_SLEEP (Process* process, const Variable* args, int argc);


int std_TIME (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    int* val = malloc(sizeof(int));

    *val = time(NULL);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_DATE (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    char* val = malloc(sizeof(char) * 11);

    time_t t = time(NULL);
    if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 0});
        if (cRes) return cRes;
        t = *(long long*)args[0].value;
    }
    struct tm tm = *localtime(&t);

    sprintf(val, "%d-%d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_DATETIME (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    char* val = malloc(sizeof(char) * 20);

    time_t t = time(NULL);
    if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 0});
        if (cRes) return cRes;
        t = *(long long*)args[0].value;
    }
    struct tm tm = *localtime(&t);

    sprintf(val, "%d-%d-%d %d:%d:%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_TIMESTAMP (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    char* val = malloc(sizeof(char) * 20);

    time_t t = time(NULL);
    if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 0});
        if (cRes) return cRes;
        t = *(long long*)args[0].value;
    }
    struct tm tm = *localtime(&t);

    sprintf(val, "%d:%d:%d", tm.tm_hour, tm.tm_min, tm.tm_sec);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_CLOCK (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    long long int* val = malloc(sizeof(long long int));

    *val = (long long int)clock();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_LONG, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_SLEEP (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_DOUBLE, 0});
    if (cRes) return cRes;

    double t = *(double*)args[0].value;

    clock_t start = clock();
    while ((double)(clock() - start) / CLOCKS_PER_SEC < t);

    return 0; // return code
}
#endif
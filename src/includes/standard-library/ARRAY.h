#ifndef ARRAY_H
#define ARRAY_H

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

int std_ARRAYSHIFT (Process* process, const Variable* args, int argc);

int std_ARRAYREMOVE (Process* process, const Variable* args, int argc);

int std_ARRAYINSERT (Process* process, const Variable* args, int argc);

int std_ARRAYSLICE (Process* process, const Variable* args, int argc);

int std_ARRAYSHIFT (Process* process, const Variable* args, int argc) {
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }


    int len = getVariablesLength(args[0].value);

    if (len <= 0) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    Variable* newArr = malloc(sizeof(Variable) * (len)); // new array
    for (int i = 1; i < len; i++) {
        newArr[i - 1] = cloneVariable(&((Variable*)args[0].value)[i]);
    }

    newArr[len-1] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ARRAYREMOVE (Process* process, const Variable* args, int argc) {
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes2) return cRes2;

    int cRes3 = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
    if (cRes3) return cRes3;

    int start = *(int*)args[1].value;
    int amount = *(int*)args[2].value;

    if (start < 0 || amount < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    int len = getVariablesLength(args[0].value);

    if (amount + start > len || start > len) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    Variable* newArr = malloc(sizeof(Variable) * (len - amount + 1)); // new array

    for (int i = 0; i < start; i++) {
        newArr[i] = cloneVariable(&((Variable*)args[0].value)[i]);
    }

    for (int i = start + amount; i < len; i++) {
        newArr[i - amount] = cloneVariable(&((Variable*)args[0].value)[i]);
    }

    newArr[len - amount] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);


    return 0; // return code
}

int std_ARRAYINSERT (Process* process, const Variable* args, int argc) {
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes2) return cRes2;

    int cRes3 = castValue((Variable*)&args[2], (Type){args[0].type.dataType, args[0].type.array-1});
    if (cRes3) return cRes3;

    int start = *(int*)args[1].value;

    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    int len = getVariablesLength(args[0].value);

    if (start > len) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    Variable* newArr = malloc(sizeof(Variable) * (len + 2)); // new array

    for (int i = 0; i < start; i++) {
        newArr[i] = cloneVariable(&((Variable*)args[0].value)[i]);
    }
    newArr[start] = cloneVariable(&args[2]);
    for (int i = start; i < len; i++) {
        newArr[i + 1] = cloneVariable(&((Variable*)args[0].value)[i]);
    }

    newArr[len + 1] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ARRAYSLICE (Process* process, const Variable* args, int argc) {
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes2) return cRes2;

    int cRes3 = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
    if (cRes3) return cRes3;

    int start = *(int*)args[1].value;
    int amount = *(int*)args[2].value;

    if (start < 0 || amount < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    int len = getVariablesLength(args[0].value);

    if (amount + start > len || start > len) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    Variable* newArr = malloc(sizeof(Variable) * (amount + 1)); // new array

    for (int i = 0; i < amount; i++) {
        newArr[i] = cloneVariable(&((Variable*)args[0].value)[i + start]);
    }

    newArr[amount] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

#endif
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

int std_ARRAYFINDINDEX (Process* process, const Variable* args, int argc);

int std_ARRAYLASTINDEX (Process* process, const Variable* args, int argc);

int std_ARRAYCONTAINS (Process* process, const Variable* args, int argc);

int std_ARRAYREVERSE (Process* process, const Variable* args, int argc);

int std_RANGE (Process* process, const Variable* args, int argc);

int std_RANGEF (Process* process, const Variable* args, int argc);

int std_FILL (Process* process, const Variable* args, int argc);

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

int std_ARRAYFINDINDEX (Process* process, const Variable* args, int argc) {
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){args[0].type.dataType, args[0].type.array-1});
    if (cRes2) return cRes2;

    int len = getVariablesLength(args[0].value);

    for (int i = 0; i < len; i++) {
        if (compareVariables(&((Variable*)args[0].value)[i], (Variable*)&args[1])) {
            Variable* var = malloc(sizeof(Variable));

            int* val = malloc(sizeof(int));
            *val = i;
            *var = createVariable("-lit", TYPE_INT, val, 0, 0);

            setReturnValue(process, var);

            destroyVariable(var);
            free(var);

            return 0; // return code
        }
    }

    Variable* var = malloc(sizeof(Variable));

    int* val = malloc(sizeof(int));
    *val = -1;

    *var = createVariable("-lit", TYPE_INT, val, 0, 0);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ARRAYLASTINDEX (Process* process, const Variable* args, int argc) {
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){args[0].type.dataType, args[0].type.array-1});
    if (cRes2) return cRes2;

    int len = getVariablesLength(args[0].value);

    for (int i = len - 1; i >= 0; i--) {
        if (compareVariables(&((Variable*)args[0].value)[i], (Variable*)&args[1])) {
            Variable* var = malloc(sizeof(Variable));

            int* val = malloc(sizeof(int));
            *val = i;
            *var = createVariable("-lit", TYPE_INT, val, 0, 0);

            setReturnValue(process, var);

            destroyVariable(var);
            free(var);

            return 0; // return code
        }
    }

    Variable* var = malloc(sizeof(Variable));

    int* val = malloc(sizeof(int));
    *val = -1;

    *var = createVariable("-lit", TYPE_INT, val, 0, 0);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ARRAYCONTAINS (Process* process, const Variable* args, int argc) {
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!args[0].type.array) {
        return ERROR_TYPE_MISMATCH;
    }

    int cRes2 = castValue((Variable*)&args[1], (Type){args[0].type.dataType, args[0].type.array-1});
    if (cRes2) return cRes2;

    int len = getVariablesLength(args[0].value);

    for (int i = 0; i < len; i++) {
        if (compareVariables(&((Variable*)args[0].value)[i], (Variable*)&args[1])) {
            Variable* var = malloc(sizeof(Variable));

            int* val = malloc(sizeof(int));
            *val = 1;
            *var = createVariable("-lit", TYPE_BOOL, val, 0, 0);

            setReturnValue(process, var);

            destroyVariable(var);
            free(var);

            return 0; // return code
        }
    }

    Variable* var = malloc(sizeof(Variable));

    int* val = malloc(sizeof(int));
    *val = 0;

    *var = createVariable("-lit", TYPE_BOOL, val, 0, 0);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ARRAYREVERSE (Process* process, const Variable* args, int argc) {
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

    Variable* newArr = malloc(sizeof(Variable) * (len + 1)); // new array

    for (int i = 0; i < len; i++) {
        newArr[i] = cloneVariable(&((Variable*)args[0].value)[len - i - 1]);
    }

    newArr[len] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_RANGE (Process* process, const Variable* args, int argc) {
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    int cRes1 = castValue((Variable*)&args[0], (Type){TYPE_INT, 0});
    if (cRes1) return cRes1;

    int end = *(int*)args[0].value;
    int start = 0;
    int step = 1;

    if (argc > 1) {
        int cRes2 = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
        if (cRes2) return cRes2;

        end = *(int*)args[1].value;
        start = *(int*)args[0].value;
    }

    if (argc > 2) {
        int cRes3 = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes3) return cRes3;
        step = *(int*)args[2].value;
    }

    if (step == 0) {
        return ERROR_INVALID_NUMBER;
    }

    int arraylen = 0;
    for (int i = start; i < end; i += step) {
        arraylen++;
    }

    Variable* newArr = malloc(sizeof(Variable) * (arraylen + 1)); // new array

    int arr_i = 0;
    for (int i = start; i < end; i += step) {
        int* val = malloc(sizeof(int));
        *val = i;
        newArr[arr_i++] = createVariable("-lit", TYPE_INT, val, 0, 0);
    }

    newArr[arraylen] = createNullTerminatedVariable();  

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, newArr, 0, 1);
    

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);


    return 0; // return code
}

int std_RANGEF (Process* process, const Variable* args, int argc) {
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    int cRes1 = castValue((Variable*)&args[0], (Type){TYPE_DOUBLE, 0});
    if (cRes1) return cRes1;

    double end = *(double*)args[0].value;
    double start = 0;
    double step = 1;

    if (argc > 1) {
        int cRes2 = castValue((Variable*)&args[1], (Type){TYPE_DOUBLE, 0});
        if (cRes2) return cRes2;

        end = *(double*)args[1].value;
        start = *(double*)args[0].value;
    }

    if (argc > 2) {
        int cRes3 = castValue((Variable*)&args[2], (Type){TYPE_DOUBLE, 0});
        if (cRes3) return cRes3;
        step = *(double*)args[2].value;
    }

    if (start > end) {
        return ERROR_START_GREATER_THAN_END;
    }

    if (step == 0) {
        return ERROR_INVALID_NUMBER;
    }

    int arraylen = 0;
    for (double i = start; i < end; i += step) {
        arraylen++;
    }

    Variable* newArr = malloc(sizeof(Variable) * (arraylen + 1)); // new array

    int i = 0;
    while (start < end) {
        double* val = malloc(sizeof(double));
        *val = start;
        newArr[i] = createVariable("-lit", TYPE_DOUBLE, val, 0, 0);
        start += step;
        i++;
    }

    newArr[arraylen] = createNullTerminatedVariable();  

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_FLOAT, newArr, 0, 1);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_FILL (Process* process, const Variable* args, int argc) {
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    // number 1 is the value, this can be any type
    // number 2 is the length of the array, this must be an int

    int cRes = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes) return cRes;

    int len = *(int*)args[1].value;

    if (len < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    Variable* newArr = malloc(sizeof(Variable) * (len + 1)); // new array

    for (int i = 0; i < len; i++) {
        newArr[i] = cloneVariable(&args[0]);
    }

    newArr[len] = createNullTerminatedVariable();

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", args[0].type.dataType, newArr, 0, args[0].type.array + 1);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

#endif
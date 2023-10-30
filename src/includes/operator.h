#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "variable.h"
#include "process.h"
#include "error.h"
#include "log.h"
#include "scope.h"

/**
 * @brief Add two variables
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int add (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Check if the type can be used in an addition
 * @param type The type to check
 * @return 1 if they can be added, 0 if not
*/
int checkIfAddable (DataType type) {
    switch (type)
    {
        case TYPE_INT:
        case TYPE_UINT:
        case TYPE_SHORT:
        case TYPE_USHORT:
        case TYPE_BYTE:
        case TYPE_UBYTE:
        case TYPE_LONG:
        case TYPE_ULONG:
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
        case TYPE_STRING:
        case TYPE_CHAR:
        case TYPE_BOOL:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

int checkIfAlpha (DataType type) {
    switch (type)
    {
        case TYPE_INT:
        case TYPE_UINT:
        case TYPE_SHORT:
        case TYPE_USHORT:
        case TYPE_BYTE:
        case TYPE_UBYTE:
        case TYPE_LONG:
        case TYPE_ULONG:
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
        case TYPE_CHAR:
        case TYPE_BOOL:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}
/**
 * @brief Check if the type can be used in an addition
 * @param type The type to check
 * @return 1 if they can be added, 0 if not
*/
int checkIfUnsigned (DataType type) {
    switch (type)
    {
        case TYPE_UINT:
        case TYPE_USHORT:
        case TYPE_UBYTE:
        case TYPE_ULONG:
        case TYPE_BOOL: // bool is just an unsigned int
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

int checkIfFloating (DataType type) {
    switch (type)
    {
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            return 1;
            break;
        default:
            return 0;
            break;
    }
}
/**
 * @brief Subtract two variables
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int subtract (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Multiply two variables
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int multiply (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Divide two variables
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int divide (Variable* var, Process* process, Variable* left, Variable* right);

int modulo (Variable* var, Process* process, Variable* left, Variable* right);

int xor (Variable* var, Process* process, Variable* left, Variable* right);

int or (Variable* var, Process* process, Variable* left, Variable* right);

int and (Variable* var, Process* process, Variable* left, Variable* right);

int add (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAddable(left->type) && !checkIfAddable(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_ADDITION;
    }
    destroyVariable(var);

    if (left->type == TYPE_STRING || right->type == TYPE_STRING) {
        char* left_value = toString(left);
        char* right_value = toString(right);
        if (left_value == NULL || right_value == NULL) {
            return ERROR_CANT_CONVERT_TO_STRING;
        }
        char* value = malloc(strlen(left_value) + strlen(right_value) + 1);
        sprintf(value, "%s%s", left_value, right_value);
        free(left_value);
        free(right_value);
        *var = createVariable("-lit", TYPE_STRING, value, 1, 0);
    }
    else if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value + *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        double left_value;
        double right_value;
        double* value = malloc(sizeof(double));

        if (checkIfFloating(left->type)) {
            left_value = *(double*)left->value;
        } else {
            left_value = *(long long int*)left->value;
        }
        if (checkIfFloating(right->type)) {
            right_value = *(double*)right->value;
        } else {
            right_value = *(long long int*)right->value;
        }

        *value = left_value + right_value;
        *var = createVariable("-lit", TYPE_DOUBLE, value, 1, 0);
    }
    return 0;
}

int subtract (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_TYPE_NOT_SUBTRACTABLE;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value - *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        double left_value;
        double right_value;
        double* value = malloc(sizeof(double));

        if (checkIfFloating(left->type)) {
            left_value = *(double*)left->value;
        } else {
            left_value = *(long long int*)left->value;
        }
        if (checkIfFloating(right->type)) {
            right_value = *(double*)right->value;
        } else {
            right_value = *(long long int*)right->value;
        }

        *value = left_value - right_value;
        *var = createVariable("-lit", TYPE_DOUBLE, value, 1, 0);
    }
    return 0;
}

int multiply (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_MULTIPLICATION;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value * *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        double left_value;
        double right_value;
        double* value = malloc(sizeof(double));

        if (checkIfFloating(left->type)) {
            left_value = *(double*)left->value;
        } else {
            left_value = *(long long int*)left->value;
        }
        if (checkIfFloating(right->type)) {
            right_value = *(double*)right->value;
        } else {
            right_value = *(long long int*)right->value;
        }

        *value = left_value * right_value;
        *var = createVariable("-lit", TYPE_DOUBLE, value, 1, 0);
    }
    return 0;
}

int divide (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_DIVISION;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value / *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        double left_value;
        double right_value;
        double* value = malloc(sizeof(double));

        if (checkIfFloating(left->type)) {
            left_value = *(double*)left->value;
        } else {
            left_value = *(long long int*)left->value;
        }
        if (checkIfFloating(right->type)) {
            right_value = *(double*)right->value;
        } else {
            right_value = *(long long int*)right->value;
        }

        *value = left_value / right_value;
        *var = createVariable("-lit", TYPE_DOUBLE, value, 1, 0);
    }
    return 0;
}

int modulo (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_MODULO;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value % *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        return ERROR_CANT_USE_TYPE_IN_MODULO;
    }
    return 0;
}

int xor (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value ^ *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    return 0;
}

int or (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value | *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    return 0;
}

int and (Variable* var, Process* process, Variable* left, Variable* right) {
    if (!checkIfAlpha(left->type) && !checkIfAlpha(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    destroyVariable(var);

    if (!checkIfFloating(left->type) && !checkIfFloating(right->type)) {
        long long int* left_value = (long long int*)left->value;
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = *left_value & *right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    return 0;
}
#endif

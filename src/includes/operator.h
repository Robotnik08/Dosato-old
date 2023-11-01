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

int checkIfNumber (DataType type) {
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
 * @brief Add two variables +
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int add (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Subtract two variables -
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int subtract (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Multiply two variables *
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int multiply (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Divide two variables /
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int divide (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief Modulo two variables %
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int modulo (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief XOR two variables ^ (bitwise)
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int xor (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief OR two variables | (bitwise)
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int or (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief AND two variables &
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int and (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief OR two variables || (logical)
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int logic_or (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief AND two variables && (logical)
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/

int logic_and (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief EQUAL two variables ==
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int equal (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief NOT EQUAL two variables !=
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int not_equal (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief LESS THAN two variables <
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int less_than (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief GREATER THAN two variables >
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int greater_than (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief LESS THAN OR EQUAL two variables <=
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/

int less_than_or_equal (Variable* var, Process* process, Variable* left, Variable* right);

/**
 * @brief GREATER THAN OR EQUAL two variables >=
 * @param var The return variable
 * @param process The process to run
 * @param left The left variable
 * @param right The right variable
*/
int greater_than_or_equal (Variable* var, Process* process, Variable* left, Variable* right);

//    UNARY OPERATORS

/**
 * @brief NOT a variable !
 * @param var The return variable
 * @param process The process to run
 * @param right The right variable
*/
int not (Variable* var, Process* process, Variable* right);

/**
 * @brief NOT a variable ~ (bitwise)
 * @param var The return variable
 * @param process The process to run
 * @param right The right variable
*/
int not_bitwise (Variable* var, Process* process, Variable* right);

/**
 * @brief NEGATE a variable -
 * @param var The return variable
 * @param process The process to run
 * @param right The right variable
*/
int negative (Variable* var, Process* process, Variable* right);

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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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
    if (!checkIfNumber(left->type) && !checkIfNumber(right->type)) {
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

int not (Variable* var, Process* process, Variable* right) {
    destroyVariable(var);

    int* value = malloc(sizeof(int));
    if (checkIfFloating(right->type)) {
        double right_value;

        right_value = *(double*)right->value;

        *value = !right_value;
        *var = createVariable("-lit", TYPE_BOOL, value, 1, 0);
    } else if (right->type == TYPE_STRING) {
        char* right_value = (char*)right->value;
        *value = !strlen(right_value);
        *var = createVariable("-lit", TYPE_BOOL, value, 1, 0);
    } else {
        long long int* right_value = (long long int*)right->value;
        *value = !*right_value;
        *var = createVariable("-lit", TYPE_BOOL, value, 1, 0);
    }
    return 0;
}

int not_bitwise (Variable* var, Process* process, Variable* right) {
    if (!checkIfNumber(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    destroyVariable(var);

    if (!checkIfFloating(right->type)) {
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = ~*right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        return ERROR_CANT_USE_TYPE_IN_BITWISE_EXPRESSION;
    }
    return 0;
}

int negative (Variable* var, Process* process, Variable* right) {
    if (!checkIfNumber(right->type)) {
        return ERROR_CANT_USE_TYPE_IN_NEGATION;
    }
    destroyVariable(var);

    if (!checkIfFloating(right->type)) {
        long long int* right_value = (long long int*)right->value;
        long long int* value = malloc(sizeof(long long int));
        *value = -*right_value;
        *var = createVariable("-lit", TYPE_LONG, value, 1, 0);
    } else {
        double right_value = *(double*)right->value;
        double* value = malloc(sizeof(double));

        *value = -right_value;
        *var = createVariable("-lit", TYPE_DOUBLE, value, 1, 0);
    }
    return 0;
}

int logic_or (Variable* var, Process* process, Variable* left, Variable* right) {
    // destroyVariable(var);

    int left_value;
    int right_value;

    if (checkIfFloating(left->type)) {
        left_value = *(double*)left->value;
    } else if (left->type == TYPE_STRING) {
        left_value = strlen((char*)left->value);
    } else {
        left_value = *(int*)left->value;
    }
    if (checkIfFloating(right->type)) {
        right_value = *(double*)right->value;
    } else if (right->type == TYPE_STRING) {
        right_value = strlen((char*)right->value);
    } else {
        right_value = *(int*)right->value;
    }

    int* value = malloc(sizeof(int));
    *value = left_value || right_value;
    *var = createVariable("-lit", TYPE_BOOL, value, 1, 0);
    return 0;
}

int logic_and (Variable* var, Process* process, Variable* left, Variable* right) {
    destroyVariable(var);

    int left_value = 0;
    int right_value = 0;

    if (checkIfFloating(left->type)) {
        left_value = *(double*)left->value;
    } else if (left->type == TYPE_STRING) {
        left_value = strlen((char*)left->value);
    } else {
        left_value = *(int*)left->value;
    }
    if (checkIfFloating(right->type)) {
        right_value = *(double*)right->value;
    } else if (right->type == TYPE_STRING) {
        right_value = strlen((char*)right->value);
    } else {
        right_value = *(int*)right->value;
    }

    int* value = malloc(sizeof(int));
    *value = left_value && right_value;
    *var = createVariable("-lit", TYPE_BOOL, value, 1, 0);
    return 0;
}

int equal (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}

int not_equal (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}

int less_than (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}

int greater_than (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}

int less_than_or_equal (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}

int greater_than_or_equal (Variable* var, Process* process, Variable* left, Variable* right) {
    return 0;
}
#endif

/**
 * @author Sebastiaan Heins
 * @file variable.h
 * @brief The variable struct and helper functions.
 * @version 0.5
 * @date 17-10-2023
*/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "strtools.h"

typedef struct {
    char* name;
    DataType type;
    int is_array;
    void* value;
    int constant;
} Variable;

/**
 * @brief Create a variable
 * @param name The name of the variable
 * @param type The type of the variable
 * @param valueptr The pointer to the value of the variable
 * @param constant Whether or not the variable is constant
 * @param array Whether or not the variable is an array (0 means no array, everything above 1 means array, 2 means array of arrays, etc.)
 * @return The variable
*/
Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant, int array);

/**
 * @brief Create a null terminated variable
 * @return The null terminated variable
*/
Variable createNullTerminatedVariable ();

/**
 * @brief Get the length of a list of variables
 * @param list The list of variables
 * @return The length of the list
*/
int getVariablesLength (const Variable* list);

/**
 * @brief Destroys a variable, freeing all memory
 * @param variable The variable to destroy
 * 
*/
void destroyVariable (Variable* variable);

/**
 * @brief Convert a variable to a string
 * @param variable The variable to convert
 * @return The string
*/
char* toString (Variable* variable);

Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant, int array) {
    Variable variable;
    variable.name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(variable.name, name);
    variable.type = type;
    variable.is_array = array;
    variable.value = valueptr;
    variable.constant = constant;
    return variable;
}

Variable createNullTerminatedVariable () {
    Variable variable;
    variable.type = D_NULL;
    variable.name = NULL;
    return variable;
}

int getVariablesLength (const Variable* list) {
    int length = 0;
    while (list[length].name != NULL) {
        length++;
    }
    return length;
}

void destroyVariable (Variable* variable) {
    // if the variable is already destroyed, return
    if (variable->name == NULL) {
        return;
    }
    free(variable->name);
    variable->name = NULL;

    switch (variable->type) {
        default:
            free(variable->value);
            break;
    }
}

char* toString (Variable* variable) {
    switch (variable->type) {
        case TYPE_CHAR:
            return (char*)variable->value;
        case TYPE_STRING:
            return (char*)variable->value;
        case TYPE_BOOL:
            return *(int*)variable->value ? "TRUE" : "FALSE";
        case TYPE_BYTE:
        case TYPE_SHORT:
        case TYPE_INT:
        case TYPE_LONG:
            return itos(*(long long*)variable->value);
        case TYPE_UBYTE:
        case TYPE_USHORT:
        case TYPE_UINT:
        case TYPE_ULONG:
            return uitos(*(unsigned long long*)variable->value);
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            return ftos(*(double*)variable->value);
        default:
            return NULL;
    }
}

#endif
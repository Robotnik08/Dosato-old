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

#endif
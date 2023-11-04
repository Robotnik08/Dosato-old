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
 * @return The string (must be freed after use)
*/
char* toString (Variable* variable);

/**
 * @brief Clone a variable
 * @param variable The variable to clone
 * @return The cloned variable
*/
Variable cloneVariable (Variable* variable);

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
    variable.value = NULL;
    variable.constant = 0;
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
    // free(variable->name);
    variable->name = NULL;

    switch (variable->type) {
        default:
            free(variable->value);
            variable->value = NULL;
            break;
    }
}

char* toString (Variable* variable) {
    char* str = NULL;

    char* res = NULL;
    switch (variable->type) {
        case TYPE_CHAR:
            str = malloc(sizeof(char) * 2);
            strcpy(str, (char*)variable->value);
            break;
        case TYPE_STRING:
            str = malloc(sizeof(char) * (strlen((char*)variable->value) + 1));
            strcpy(str, (char*)variable->value);
            break;
        case TYPE_BOOL:
            str = malloc(sizeof(char) * 6);
            strcpy(str, *(int*)variable->value ? "TRUE" : "FALSE");
            break;
        case TYPE_BYTE:
        case TYPE_SHORT:
        case TYPE_INT:
        case TYPE_LONG:
            res = itos(*(long long*)variable->value);
            str = malloc(sizeof(char) * (strlen(res) + 1));
            strcpy(str, res);
            break;
        case TYPE_UBYTE:
        case TYPE_USHORT:
        case TYPE_UINT:
        case TYPE_ULONG:
            res = uitos(*(unsigned long long*)variable->value);
            str = malloc(sizeof(char) * (strlen(res) + 1));
            strcpy(str, res);
            break;
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            res = ftos(*(double*)variable->value);
            str = malloc(sizeof(char) * (strlen(res) + 1));
            strcpy(str, res);
            break;
        default:
            str = NULL;
            break;
    }
    return str;
}

Variable cloneVariable (Variable* variable) {
    Variable new_variable = createVariable("lit", variable->type, NULL, variable->constant, variable->is_array);

    switch (variable->type) {
        case TYPE_CHAR:
            new_variable.value = malloc(sizeof(char));
            *(char*)new_variable.value = *(char*)variable->value;
            break;
        case TYPE_STRING:
            new_variable.value = malloc(sizeof(char) * (strlen((char*)variable->value) + 1));
            strcpy((char*)new_variable.value, (char*)variable->value);
            break;
        case TYPE_BOOL:
            new_variable.value = malloc(sizeof(int));
            *(int*)new_variable.value = *(int*)variable->value;
            break;
        case TYPE_BYTE:
        case TYPE_SHORT:
        case TYPE_INT:
        case TYPE_LONG:
            new_variable.value = malloc(sizeof(long long int));
            *(long long int*)new_variable.value = *(long long int*)variable->value;
            break;
        case TYPE_UBYTE:
        case TYPE_USHORT:
        case TYPE_UINT:
        case TYPE_ULONG:
            new_variable.value = malloc(sizeof(unsigned long long int));
            *(unsigned long long int*)new_variable.value = *(unsigned long long int*)variable->value;
            break;
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            new_variable.value = malloc(sizeof(double));
            *(double*)new_variable.value = *(double*)variable->value;
            break;
        default:
            new_variable.value = NULL;
            break;
    }
    return new_variable;
}

int getIfCastable (DataType a, DataType b) {
    if (a == b) {
        return 1;
    }
    switch (a) {
        case TYPE_INT:
        case TYPE_BYTE:
        case TYPE_SHORT:
        case TYPE_LONG:
        case TYPE_UINT:
        case TYPE_UBYTE:
        case TYPE_USHORT:
        case TYPE_ULONG:
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
            if (b == TYPE_BYTE || b == TYPE_SHORT || b == TYPE_LONG || b == TYPE_INT || b == TYPE_UBYTE || b == TYPE_USHORT || b == TYPE_ULONG || b == TYPE_UINT || b == TYPE_FLOAT || b == TYPE_DOUBLE) {
                return 1;
            }
            break;
        default:
            break;
        
    }
    return 0;
}

long long int getSignedNumber (Variable* variable) {
    switch (variable->type) {
        case TYPE_BYTE:
        case TYPE_UBYTE:
            return *(char*)variable->value;
        case TYPE_SHORT:
        case TYPE_USHORT:
            return *(short*)variable->value;
        case TYPE_INT:
        case TYPE_UINT:
            return *(int*)variable->value;
        case TYPE_LONG:
        case TYPE_ULONG:
            return *(long long*)variable->value;
        case TYPE_FLOAT:
            return (long long int)(*(float*)variable->value);
        case TYPE_DOUBLE:
            return (long long int)*(double*)variable->value;
        default:
            return 0;
    }
}
unsigned long long int getUnsignedNumber (Variable* variable) {
    switch (variable->type) {
        case TYPE_BYTE:
        case TYPE_UBYTE:
            return *(unsigned char*)variable->value;
        case TYPE_SHORT:
        case TYPE_USHORT:
            return *(unsigned short*)variable->value;
        case TYPE_INT:
        case TYPE_UINT:
            return *(unsigned int*)variable->value;
        case TYPE_LONG:
        case TYPE_ULONG:
            return *(unsigned long long*)variable->value;
        case TYPE_FLOAT:
            return (long long int)(*(float*)variable->value); // casting into a signed int, it'll be casted back to unsigned in the return
        case TYPE_DOUBLE:
            return (long long int)(*(double*)variable->value); // casting into a signed int, it'll be casted back to unsigned in the return
        default:
            return 0;
    }
}
double getFloatNumber (Variable* variable) {
    switch (variable->type) {
        case TYPE_FLOAT:
            return *(float*)variable->value;
        case TYPE_DOUBLE:
            return *(double*)variable->value;
        default:
            return 0;
    }
}

int castValue (Variable* variable, DataType type) {
    if (variable->type == type) {
        return 0;
    }
    if (!getIfCastable(variable->type, type)) {
        return 1;
    }
    

    void* new_value = NULL;
    switch (type) {
        case TYPE_BYTE:
            new_value = malloc(sizeof(char));
            *(signed char*)new_value = getSignedNumber(variable);
            variable->type = TYPE_BYTE;
            break;
        case TYPE_SHORT:
            new_value = malloc(sizeof(short));
            *(short*)new_value = getSignedNumber(variable);
            variable->type = TYPE_SHORT;
            break;
        case TYPE_INT:
            new_value = malloc(sizeof(int));
            *(int*)new_value = getSignedNumber(variable);
            variable->type = TYPE_INT;
            break;
        case TYPE_LONG:
            new_value = malloc(sizeof(long long int));
            *(long long int*)new_value = getSignedNumber(variable);
            variable->type = TYPE_LONG;
            break;
        case TYPE_UBYTE:
            new_value = malloc(sizeof(unsigned char));
            *(unsigned char*)new_value = getUnsignedNumber(variable);
            variable->type = TYPE_UBYTE;
            break;
        case TYPE_USHORT:
            new_value = malloc(sizeof(unsigned short));
            *(unsigned short*)new_value = getUnsignedNumber(variable);
            variable->type = TYPE_USHORT;
            break;
        case TYPE_UINT:
            new_value = malloc(sizeof(unsigned int));
            *(unsigned int*)new_value = getUnsignedNumber(variable);
            variable->type = TYPE_UINT;
            break;
        case TYPE_ULONG:
            new_value = malloc(sizeof(unsigned long long int));
            *(unsigned long long int*)new_value = getUnsignedNumber(variable);
            variable->type = TYPE_ULONG;
            break;
        case TYPE_FLOAT:
            new_value = malloc(sizeof(float));
            *(float*)new_value = getFloatNumber(variable);
            variable->type = TYPE_FLOAT;
            break;
        case TYPE_DOUBLE:
            new_value = malloc(sizeof(double));
            *(double*)new_value = getFloatNumber(variable);
            variable->type = TYPE_DOUBLE;
            break;
    }
    free(variable->value);
    variable->value = new_value;
    return 0;
}

#endif
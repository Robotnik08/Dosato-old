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
    DataType dataType;
    int isArray;
} Type;

typedef struct {
    char* name;
    Type type;
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

/**
 * @brief Get whether or not a variable can be casted to another type
 * @param a The type of the variable
 * @param b The type to cast to
 * @return Whether or not the variable can be casted to the other type
*/
int getIfCastable (DataType a, DataType b);

/**
 * @brief Get the signed number of a variable
 * @param variable The variable to get the number of
 * @return The signed number
*/
long long int getSignedNumber (Variable* variable);

/**
 * @brief Get the unsigned number of a variable
 * @param variable The variable to get the number of
 * @return The unsigned number
*/
unsigned long long int getUnsignedNumber (Variable* variable);

/**
 * @brief Get the float number of a variable
 * @param variable The variable to get the number of
 * @return The float number
*/
double getFloatNumber (Variable* variable);

/**
 * @brief Cast a variable to a type
 * @param variable The variable to cast
 * @param type The type to cast to
 * @return The error code
*/
int castValue (Variable* variable, DataType type);

/**
 * @brief Cast an array to a type
 * @param variable The variable to cast
 * @param type The type to cast to
 * @return The error code
*/
int castArray (Variable* variable, DataType type);

/**
 * @brief Compare the type of two variables (including arrays)
 * @param left The left variable
 * @param right The right variable
 * @return Whether or not the types are equal
*/
int compareType (Variable* left, Variable* right);

Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant, int array) {
    Variable variable;
    variable.name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(variable.name, name);
    variable.type = (Type){type, array};
    variable.value = valueptr;
    variable.constant = constant;
    return variable;
}

Variable createNullTerminatedVariable () {
    Variable variable;
    variable.type = (Type){D_NULL, 0};
    variable.name = NULL;
    variable.value = NULL;
    variable.constant = 0;
    return variable;
}

int getVariablesLength (const Variable* list) {
    int length = 0;
    while (list[length].type.dataType != D_NULL) {
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

    if (variable->type.isArray) {
        Variable* array = (Variable*)variable->value;
        int array_length = getVariablesLength(array);
        for (int i = 0; i < array_length; i++) {
            destroyVariable(&array[i]);
        }
    }

    switch (variable->type.dataType) {
        default:
            free(variable->value);
            variable->value = NULL;
            break;
    }
}

void destroyVariableRef (Variable* variable) {
    if (variable->name == NULL) {
        return;
    }
    free(variable->name);
    variable->name = NULL;
}

char* toString (Variable* variable) {
    char* str = NULL;

    char* res = NULL;
    switch (variable->type.dataType) {
        case TYPE_CHAR:
            str = malloc(sizeof(char) * 2);
            str[0] = ((char*)variable->value)[0];
            str[1] = '\0';
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
    Variable new_variable = createVariable("-lit", variable->type.dataType, NULL, variable->constant, variable->type.isArray);

    switch (variable->type.dataType) {
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
            new_variable.value = variable->value;
            free(new_variable.name); // the value will be the exact same memory, it cannot be freed
            new_variable.name = malloc(sizeof(char) * (strlen("-lit") + strlen("del") + 1));
            strcpy(new_variable.name, "-lit");
            strcat(new_variable.name, "del");
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
        case TYPE_STRING:
        case TYPE_CHAR:
        case TYPE_BOOL:
            if (b == TYPE_BYTE || b == TYPE_SHORT || b == TYPE_LONG || b == TYPE_INT || b == TYPE_UBYTE || b == TYPE_USHORT || b == TYPE_ULONG || b == TYPE_UINT || b == TYPE_FLOAT || b == TYPE_DOUBLE || b == TYPE_STRING || b == TYPE_CHAR || b == TYPE_BOOL) {
                return 1;
            }
            break;
        default:
            break;
        
    }
    return 0;
}

long long int getSignedNumber (Variable* variable) {
    switch (variable->type.dataType) {
        case TYPE_BOOL:
            return *(int*)variable->value != 0;
        case TYPE_BYTE:
        case TYPE_UBYTE:
        case TYPE_CHAR:
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
        case TYPE_STRING:
            return strlen((char*)variable->value);
        default:
            return 0;
    }
}
unsigned long long int getUnsignedNumber (Variable* variable) {
    switch (variable->type.dataType) {
        case TYPE_BOOL:
            return *(int*)variable->value != 0;
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
        case TYPE_STRING:
            return strlen((char*)variable->value);
        default:
            return 0;
    }
}
double getFloatNumber (Variable* variable) {
    switch (variable->type.dataType) {
        case TYPE_BOOL:
            return (double)(*(int*)variable->value != 0);
        case TYPE_BYTE:
        case TYPE_UBYTE:
            return (double)(*(unsigned char*)variable->value);
        case TYPE_SHORT:
        case TYPE_USHORT:
            return (double)(*(unsigned short*)variable->value);
        case TYPE_INT:
        case TYPE_UINT:
            return (double)(*(unsigned int*)variable->value);
        case TYPE_LONG:
        case TYPE_ULONG:
            return (double)(*(unsigned long long*)variable->value);
        case TYPE_FLOAT:
            return *(float*)variable->value; // casting into a signed int, it'll be casted back to unsigned in the return
        case TYPE_DOUBLE:
            return *(double*)variable->value; // casting into a signed int, it'll be casted back to unsigned in the return
        case TYPE_STRING:
            return (double)strlen((char*)variable->value);
        default:
            return 0;
    }
}

int castValue (Variable* variable, DataType type) {
    if (variable->type.isArray) {
        return castArray(variable, type);
    }
    if (variable->type.dataType == type) {
        return 0;
    }
    if (!getIfCastable(variable->type.dataType, type)) {
        return ERROR_CAST_ERROR;
    }
    

    void* new_value = NULL;
    switch (type) {
        case TYPE_BYTE:
            new_value = malloc(sizeof(char));
            *(signed char*)new_value = getSignedNumber(variable);
            variable->type.dataType = TYPE_BYTE;
            break;
        case TYPE_SHORT:
            new_value = malloc(sizeof(short));
            *(short*)new_value = getSignedNumber(variable);
            variable->type.dataType = TYPE_SHORT;
            break;
        case TYPE_INT:
            new_value = malloc(sizeof(int));
            *(int*)new_value = getSignedNumber(variable);
            variable->type.dataType = TYPE_INT;
            break;
        case TYPE_LONG:
            new_value = malloc(sizeof(long long int));
            *(long long int*)new_value = getSignedNumber(variable);
            variable->type.dataType = TYPE_LONG;
            break;
        case TYPE_CHAR:
        case TYPE_UBYTE:
            new_value = malloc(sizeof(unsigned char));
            *(unsigned char*)new_value = getUnsignedNumber(variable);
            variable->type.dataType = type;
            break;
        case TYPE_USHORT:
            new_value = malloc(sizeof(unsigned short));
            *(unsigned short*)new_value = getUnsignedNumber(variable);
            variable->type.dataType = TYPE_USHORT;
            break;
        case TYPE_UINT:
            new_value = malloc(sizeof(unsigned int));
            *(unsigned int*)new_value = getUnsignedNumber(variable);
            variable->type.dataType = TYPE_UINT;
            break;
        case TYPE_ULONG:
            new_value = malloc(sizeof(unsigned long long int));
            *(unsigned long long int*)new_value = getUnsignedNumber(variable);
            variable->type.dataType = TYPE_ULONG;
            break;
        case TYPE_FLOAT:
            new_value = malloc(sizeof(float));
            *(float*)new_value = getFloatNumber(variable);
            variable->type.dataType = TYPE_FLOAT;
            break;
        case TYPE_DOUBLE:
            new_value = malloc(sizeof(double));
            *(double*)new_value = getFloatNumber(variable);
            variable->type.dataType = TYPE_DOUBLE;
            break;
        case TYPE_BOOL:
            new_value = malloc(sizeof(int));
            *(int*)new_value = getSignedNumber(variable) != 0;
            variable->type.dataType = TYPE_BOOL;
            break;
        case TYPE_STRING:
            return ERROR_CAST_ERROR; // casting into a string is not fine, but casting from a string is fine
    }
    free(variable->value);
    variable->value = new_value;
    return 0;
}

int castArray (Variable* variable, DataType type) {
    if (variable->type.dataType == type) {
        return 0;
    }
    if (!getIfCastable(variable->type.dataType, type) && variable->type.dataType != TYPE_ARRAY) {
        return ERROR_CAST_ERROR;
    }
    int array_length = getVariablesLength((Variable*)variable->value);
    int array_depth = variable->type.isArray;

    Variable* array = (Variable*)variable->value;
    for (int i = 0; i < array_length; i++) {
        if (array[i].type.isArray + 1 != array_depth) {
            return ERROR_INCORRECT_ARRAY_DEPTH; // if the depth of the array is not equal to the depth of the variable, return an error, because of multidimensional arrays
        }
        int cRes = castValue(&array[i], type);

        if (cRes) return cRes;
    }
    return 0;
}

int compareType (Variable* left, Variable* right) {
    return left->type.dataType != right->type.dataType || left->type.isArray != right->type.isArray;
}

int getTypeFromCastNode (Type* t, Node* cast) {
    if (cast->type != NODE_OPERATOR_CAST) return ERROR_INVALID_CAST;
    if (cast->start + 1 >= cast->end) return ERROR_INVALID_CAST;
    
    for (int i = cast->start + 1; i < cast->end-1; i++) {
        if (getTokenAtPosition(NULL, i).carry == TYPE_ARRAY) {
            t->isArray++;
        } else {
            return ERROR_INVALID_CAST;
        }
    }
    if (getTokenAtPosition(NULL, cast->end-1).carry == TYPE_ARRAY) {
        return ERROR_INVALID_CAST;
    } else {
        t->dataType = getTokenAtPosition(NULL, cast->end-1).carry;
    }
    
    return 0;
}

#endif
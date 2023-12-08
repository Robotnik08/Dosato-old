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
    int array;
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
Variable cloneVariable (const Variable* variable);

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
int castValue (Variable* variable, Type type);

/**
 * @brief Cast an array to a type
 * @param variable The variable to cast
 * @param type The type to cast to
 * @return The error code
*/
int castArray (Variable* variable, Type type);

/**
 * @brief Compare the type of two variables (including arrays)
 * @param left The left variable
 * @param right The right variable
 * @return Whether or not the types are equal
*/
int compareType (Type left, Type right);

int printType (Type t);

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

    if (variable->type.array) {
        Variable* array = (Variable*)variable->value;
        int array_length = getVariablesLength(array);
        for (int i = 0; i < array_length; i++) {
            destroyVariable(&array[i]);
        }
    }

    switch (variable->type.dataType) {
        default:
            if (variable->value == NULL) return;
            free(variable->value);
            variable->value = NULL;
            break;
    }
}

void destroyValue (Variable* variable) {
    if (variable->type.array) {
        Variable* array = (Variable*)variable->value;
        int array_length = getVariablesLength(array);
        for (int i = 0; i < array_length; i++) {
            destroyVariable(&array[i]);
        }
    }

    switch (variable->type.dataType) {
        default:
            if (variable->value == NULL) return;
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
    
    if (variable->type.array) {
        int array_length = getVariablesLength((Variable*)variable->value);
        str = malloc(sizeof(char) * 3);
        strcpy(str, "[");
        for (int i = 0; i < array_length; i++) {
            char* res = toString(&((Variable*)variable->value)[i]);
            str = realloc(str, sizeof(char) * (strlen(str) + strlen(res) + 3)); // 3 for the ", " and the "]"
            strcat(str, res);
            if (i < array_length - 1) strcat(str, ", ");
            free(res);
        }

        strcat(str, "]");
        return str;
    }

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
            str = itos(*(signed char*)variable->value);
            break;
        case TYPE_SHORT:
            str = itos(*(short*)variable->value);
            break;
        case TYPE_INT:
            str = itos(*(int*)variable->value);
            break;
        case TYPE_LONG:
            str = itos(*(long long*)variable->value);
            break;
        case TYPE_UBYTE:
            str = uitos(*(unsigned char*)variable->value);
            break;
        case TYPE_USHORT:
            str = uitos(*(unsigned short*)variable->value);
            break;
        case TYPE_UINT:
            str = uitos(*(unsigned int*)variable->value);
            break;
        case TYPE_ULONG:
            str = uitos(*(unsigned long long*)variable->value);
            break;
        case TYPE_FLOAT:
            str = ftos(*(float*)variable->value);
            break;
        case TYPE_DOUBLE:
            str = ftos(*(double*)variable->value);
            break;
        default:
            str = NULL;
            break;
    }
    return str;
}

Variable cloneVariable (const Variable* variable) {
    Variable new_variable = createVariable("-lit", variable->type.dataType, NULL, variable->constant, variable->type.array);
    if (!variable->type.array) {
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
                new_variable.value = malloc(sizeof(signed char));
                *(signed char*)new_variable.value = *(signed char*)variable->value;
                break;
            case TYPE_SHORT:
                new_variable.value = malloc(sizeof(short));
                *(short*)new_variable.value = *(short*)variable->value;
                break;
            case TYPE_INT:
                new_variable.value = malloc(sizeof(int));
                *(int*)new_variable.value = *(int*)variable->value;
                break;
            case TYPE_LONG:
                new_variable.value = malloc(sizeof(long long int));
                *(long long int*)new_variable.value = *(long long int*)variable->value;
                break;
            case TYPE_UBYTE:
                new_variable.value = malloc(sizeof(unsigned char));
                *(unsigned char*)new_variable.value = *(unsigned char*)variable->value;
                break;
            case TYPE_USHORT:
                new_variable.value = malloc(sizeof(unsigned short));
                *(unsigned short*)new_variable.value = *(unsigned short*)variable->value;
                break;
            case TYPE_UINT:
                new_variable.value = malloc(sizeof(unsigned int));
                *(unsigned int*)new_variable.value = *(unsigned int*)variable->value;
                break;
            case TYPE_ULONG:
                new_variable.value = malloc(sizeof(unsigned long long int));
                *(unsigned long long int*)new_variable.value = *(unsigned long long int*)variable->value;
                break;
            case TYPE_FLOAT:
                new_variable.value = malloc(sizeof(float));
                *(float*)new_variable.value = *(float*)variable->value;
                break;
            case TYPE_DOUBLE:
                new_variable.value = malloc(sizeof(double));
                *(double*)new_variable.value = *(double*)variable->value;
                break;
            default:
                // for now, nothing happens
                break;
        }
    } else {
        Variable* array = (Variable*)variable->value;
        int array_length = getVariablesLength(array);
        Variable* new_array = malloc(sizeof(Variable) * (array_length + 1));
        for (int i = 0; i < array_length; i++) {
            new_array[i] = cloneVariable(&array[i]);
        }
        new_array[array_length] = createNullTerminatedVariable();
        new_variable.value = new_array;
    }
    return new_variable;
}

void* cloneValue (const Variable* variable) {
    if (!variable->type.array) {
        switch (variable->type.dataType) {
            case TYPE_CHAR:
            case TYPE_STRING:
            case TYPE_BOOL:
            case TYPE_BYTE:
            case TYPE_SHORT:
            case TYPE_INT:
            case TYPE_LONG:
            case TYPE_UBYTE:
            case TYPE_USHORT:
            case TYPE_UINT:
            case TYPE_ULONG:
            case TYPE_FLOAT:
            case TYPE_DOUBLE:
                return (void*)variable->value;
            default:
                return NULL;
        }
    }
    Variable* array = (Variable*)variable->value;
    int array_length = getVariablesLength(array);
    Variable* new_array = malloc(sizeof(Variable) * (array_length + 1));
    for (int i = 0; i < array_length; i++) {
        new_array[i] = cloneVariable(&array[i]);
    }
    new_array[array_length] = createNullTerminatedVariable();
    return new_array;
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
    if (variable->type.array) {
        // return the length of the array
        return getVariablesLength((Variable*)variable->value);
    }
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
    if (variable->type.array) {
        // return the length of the array
        return getVariablesLength((Variable*)variable->value);
    }
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
    if (variable->type.array) {
        // return the length of the array
        return getVariablesLength((Variable*)variable->value);
    }
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
            return *(float*)variable->value;
        case TYPE_DOUBLE:
            return *(double*)variable->value;
        case TYPE_STRING:
            return (double)strlen((char*)variable->value);
        default:
            return 0;
    }
}

int castValue (Variable* variable, Type type) {
    if (variable->type.array || type.array) {
        if (variable->type.array && type.array) {
            return castArray(variable, type);
        }
        if (variable->type.array) {
            switch (type.dataType) {
                default: 
                    return ERROR_ARRAY_CAST_ERROR;
                case TYPE_STRING:
                    char* res = toString(variable);
                    void* new_value = malloc(sizeof(char) * (strlen(res) + 1));
                    strcpy((char*)new_value, res);
                    free (res);
                    char* name = malloc(sizeof(char) * (strlen(variable->name) + 1));
                    strcpy(name, variable->name);
                    destroyVariable(variable);
                    *variable = createVariable(name, TYPE_STRING, new_value, 0, 0);

                    break;
                case TYPE_BYTE:
                case TYPE_SHORT:
                case TYPE_INT:
                case TYPE_LONG:
                case TYPE_UBYTE:
                case TYPE_USHORT:
                case TYPE_UINT:
                case TYPE_ULONG:
                case TYPE_FLOAT:
                case TYPE_DOUBLE:
                case TYPE_BOOL:
                    int array_length = getVariablesLength((Variable*)variable->value);
                    int* len_val = malloc(sizeof(int));
                    *len_val = array_length;
                    Variable* val = malloc(sizeof(Variable));
                    *val = createVariable(variable->name, TYPE_INT, len_val, 0, 0);
                    int cRes = castValue(val, type);
                    if (cRes) return cRes;

                    destroyVariable(variable);
                    *variable = cloneVariable(val);
                    destroyVariable(val);
                    free(val);
                    break;
            }
            return 0;
        }

        return ERROR_ARRAY_CAST_ERROR;
    }
    if (variable->type.dataType == type.dataType) {
        return 0;
    }
    if (!getIfCastable(variable->type.dataType, type.dataType)) {
        return ERROR_CAST_ERROR;
    }
    

    void* new_value = NULL;
    switch (type.dataType) {
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
            variable->type.dataType = type.dataType;
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
            char* res = toString(variable);
            new_value = malloc(sizeof(char) * (strlen(res) + 1));
            strcpy((char*)new_value, res);
            variable->type.dataType = TYPE_STRING;
            free(res);
            break;
    }
    free(variable->value);
    variable->value = new_value;
    return 0;
}

int castArray (Variable* variable, Type type) {
    if (compareType(variable->type, type)) {
        return 0;
    }
    if (!getIfCastable(variable->type.dataType, type.dataType) && variable->type.dataType != TYPE_ARRAY) {
        return ERROR_CAST_ERROR;
    }
    int array_length = getVariablesLength((Variable*)variable->value);
    int array_depth = variable->type.array;

    Variable* array = (Variable*)variable->value;
    for (int i = 0; i < array_length; i++) {
        if (array[i].type.array + 1 != array_depth) {
            return ERROR_INCORRECT_ARRAY_DEPTH; // if the depth of the array is not equal to the depth of the variable, return an error, because of multidimensional arrays
        }
        int cRes = castValue(&array[i], (Type){type.dataType, type.array - 1});

        if (cRes) return cRes;
    }
    variable->type = type;
    return 0;
}

int compareType (Type left, Type right) {
    return left.dataType == right.dataType && left.array == right.array;
}

int printType (Type t) {
    return printf("Type: DataType %d, ArrayDepth %d\n", t.dataType, t.array);
}

int pushArray (Variable* arr, Variable* val) {
    if (arr->type.array == 0) {
        return ERROR_ARRAY_CAST_ERROR;
    }
    if (arr->value == NULL) {
        arr->value = malloc(sizeof(Variable) * 2);
        ((Variable*)arr->value)[0] = cloneVariable(val);
        int cRes = castValue(&((Variable*)arr->value)[0], (Type){arr->type.dataType, arr->type.array - 1});
        if (cRes) return cRes;
        ((Variable*)arr->value)[1] = createNullTerminatedVariable();
        return 0;
    }
    int array_length = getVariablesLength(arr->value);
    arr->value = realloc(arr->value, sizeof(Variable) * (array_length + 2));
    int cRes = castValue(val, (Type){arr->type.dataType, arr->type.array - 1});
    if (cRes) return cRes;
    ((Variable*)arr->value)[array_length] = cloneVariable(val);
    ((Variable*)arr->value)[array_length + 1] = createNullTerminatedVariable();
    return 0;
}

#endif
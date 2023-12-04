#ifndef STRING_H
#define STRING_H

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

int std_SPLIT (Process* process, const Variable* args, int argc);

int std_LOWERCASE (Process* process, const Variable* args, int argc);

int std_UPPERCASE (Process* process, const Variable* args, int argc);

int std_LENGTH (Process* process, const Variable* args, int argc);

int std_SUBSTRING (Process* process, const Variable* args, int argc);

int std_INDEXOF (Process* process, const Variable* args, int argc);

int std_LASTINDEXOF (Process* process, const Variable* args, int argc);

int std_STARTSWITH (Process* process, const Variable* args, int argc);

int std_ENDSWITH (Process* process, const Variable* args, int argc);

int std_TRIM (Process* process, const Variable* args, int argc);

int std_REVERSE (Process* process, const Variable* args, int argc);

int std_REPLACE (Process* process, const Variable* args, int argc);

int std_CONTAINS (Process* process, const Variable* args, int argc);

int std_REMOVE (Process* process, const Variable* args, int argc);

int std_INSERT (Process* process, const Variable* args, int argc);

int std_STRINGTOINT (Process* process, const Variable* args, int argc);

int std_STRINGTODOUBLE (Process* process, const Variable* args, int argc);

int std_SPLIT (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* sep = (char*)args[1].value;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 1);

    char* token = strtok(str, sep);

    while (token != NULL) {
        Variable* var2 = malloc(sizeof(Variable));
        char* val = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(val, token);
        *var2 = createVariable("-lit", TYPE_STRING, val, 0, 0);
        int pRes = pushArray(var, var2);
        if (pRes) return pRes;

        destroyVariable(var2);
        free(var2);
        token = strtok(NULL, sep);
    }

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_LOWERCASE (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* str = (char*)args[0].value;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 0);

    char* val = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(val, str);

    for (int i = 0; i < strlen(val); i++) {
        if (val[i] >= 'A' && val[i] <= 'Z') {
            val[i] += 32;
        }
    }

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_UPPERCASE (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* str = (char*)args[0].value;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 0);

    char* val = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(val, str);

    for (int i = 0; i < strlen(val); i++) {
        if (val[i] >= 'a' && val[i] <= 'z') {
            val[i] -= 32;
        }
    }

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_LENGTH (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);

    int* val = malloc(sizeof(int));
    *val = strlen((char*)args[0].value);

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_SUBSTRING (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes) return cRes;
    int start = *(int*)args[1].value;
    int end = strlen(str);
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        end = *(int*)args[2].value;
    }

    if (start < 0 || end < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    if (start > end) {
        return ERROR_START_GREATER_THAN_END;
    }

    if (start > strlen(str) || end > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 0);

    char* val = malloc(sizeof(char) * (end - start + 2));
    strncpy(val, str + start, end - start + 1);
    val[end - start + 1] = '\0';

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_INDEXOF (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* substr = (char*)args[1].value;

    int start = 0;
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[2].value;
    }

    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    char* res = strstr(str + start, substr);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);

    int* val = malloc(sizeof(int));
    if (res == NULL) {
        *val = -1;
    } else {
        *val = res - str;
    }

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_LASTINDEXOF (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* substr = (char*)args[1].value;

    int start = strlen(str) - 1;
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[2].value;
    }

    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    char* res = NULL;
    for (int i = start; i >= 0; i--) {
        if (strncmp(str + i, substr, strlen(substr)) == 0) {
            res = str + i;
            break;
        }
    }

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);

    int* val = malloc(sizeof(int));
    if (res == NULL) {
        *val = -1;
    } else {
        *val = res - str;
    }

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_STARTSWITH (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* substr = (char*)args[1].value;

    int start = 0;
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[2].value;
    }

    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    int res = strncmp(str + start, substr, strlen(substr)) == 0;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);

    int* val = malloc(sizeof(int));
    *val = res;

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_ENDSWITH (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    char* substr = (char*)args[1].value;

    int start = strlen(str) - strlen(substr);
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[2].value;
    }

    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    int res = strncmp(str + start, substr, strlen(substr)) == 0;

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);

    int* val = malloc(sizeof(int));
    *val = res;

    var->value = val;

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_TRIM (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 0);
    
    char* val = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(val, str);
    
    int start = 0;
    int end = strlen(val) - 1;
    
    while (val[start] == ' ' || val[start] == '\n' || val[start] == '\t') {
        start++;
    }
    
    while (val[end] == ' ' || val[end] == '\n' || val[end] == '\t') {
        end--;
    }
    
    val[end + 1] = '\0';
    
    char* val2 = malloc(sizeof(char) * (strlen(val + start) + 1));
    strcpy(val2, val + start);
    
    var->value = val2;
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

int std_REVERSE (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, NULL, 0, 0);
    
    char* val = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(val, str);
    
    for (int i = 0; i < strlen(val) / 2; i++) {
        char temp = val[i];
        val[i] = val[strlen(val) - i - 1];
        val[strlen(val) - i - 1] = temp;
    }
    
    var->value = val;
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

int std_REPLACE (Process* process, const Variable* args, int argc) {
    if (argc > 4) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* substr = (char*)args[1].value;
    
    cRes = castValue((Variable*)&args[2], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* repl = (char*)args[2].value;
    
    int start = 0;
    if (argc == 4) {
        cRes = castValue((Variable*)&args[3], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[3].value;
    }
    
    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }
    
    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }
    
    // replace all instances of substr with repl in str starting at start

    char* val = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(val, str);

    char* res = strstr(val + start, substr);

    while (res != NULL) {
        char* val2 = malloc(sizeof(char) * (strlen(val) - strlen(substr) + strlen(repl) + 1));
        strncpy(val2, val, res - val);
        val2[res - val] = '\0';
        strcat(val2, repl);
        strcat(val2, res + strlen(substr));
        free(val);
        val = val2;
        res = strstr(val + start, substr);
    }
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

int std_CONTAINS (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* substr = (char*)args[1].value;
    
    int start = 0;
    if (argc == 3) {
        cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        start = *(int*)args[2].value;
    }
    
    if (start < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }
    
    if (start > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }
    
    char* res = strstr(str + start, substr);
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_BOOL, NULL, 0, 0);
    
    int* val = malloc(sizeof(int));
    *val = res != NULL;
    
    var->value = val;
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

int std_REMOVE (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    cRes = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes) return cRes;
    
    int start = *(int*)args[1].value;
    
    cRes = castValue((Variable*)&args[2], (Type){TYPE_INT, 0});
    if (cRes) return cRes;
    
    int amount = *(int*)args[2].value;
    
    // remove characters in str from start to end

    if (start < 0 || amount < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }
    
    if (start > strlen(str) || start + amount > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    char* val = malloc(sizeof(char) * (strlen(str) - amount + 1));

    strncpy(val, str, start);
    val[start] = '\0';
    strcat(val, str + start + amount);

    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_INSERT (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    char* str = (char*)args[0].value;

    cRes = castValue((Variable*)&args[1], (Type){TYPE_INT, 0});
    if (cRes) return cRes;

    int index = *(int*)args[1].value;
    if (index < 0) {
        return ERROR_NUMBER_CANNOT_BE_NEGATIVE;
    }

    cRes = castValue((Variable*)&args[2], (Type){TYPE_STRING, 0});

    if (index > strlen(str)) {
        return ERROR_ARRAY_OUT_OF_BOUNDS;
    }

    char* substr = (char*)args[2].value;
    
    char* val = malloc(sizeof(char) * (strlen(str) + strlen(substr) + 1));
    
    strncpy(val, str, index);

    val[index] = '\0';
    
    strcat(val, substr);
    strcat(val, str + index);

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);

    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}

int std_STRINGTOINT (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, NULL, 0, 0);
    
    int* val = malloc(sizeof(int));
    *val = atoi(str);
    
    var->value = val;
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

int std_STRINGTODOUBLE (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;
    
    char* str = (char*)args[0].value;
    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_DOUBLE, NULL, 0, 0);
    
    double* val = malloc(sizeof(double));
    *val = atof(str);
    
    var->value = val;
    
    setReturnValue(process, var);
    
    destroyVariable(var);
    free(var);
    
    return 0; // return code
}

#endif
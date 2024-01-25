#ifndef FILE_H
#define FILE_H

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

int std_READ (Process* process, const Variable* args, int argc);

int std_WRITE (Process* process, const Variable* args, int argc);

int std_APPEND (Process* process, const Variable* args, int argc);

int std_READ (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    FILE* file = fopen ((char*)args[0].value, "r");

    if (file == NULL) {
        #ifdef _WIN32
        _fcloseall(); // close all files if any were opened
        #endif
        
        return errno == EACCES ? ERROR_PERMISSION_DENIED : ERROR_FILE_NOT_FOUND;
    }

    long long int size = 0;
    while (fgetc(file) != EOF) {
        size++;
    }
    fseek(file, 0, SEEK_SET);

    char* val = malloc(sizeof(char) * (size + 1));

    fread(val, sizeof(char), size, file);

    fclose(file);
    val[size] = '\0';

    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, val, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);


    return 0; // return code
}

int std_WRITE (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    FILE* file = fopen ((char*)args[0].value, "w");

    if (file == NULL) {
        #ifdef _WIN32
        _fcloseall(); // close all files if any were opened
        #endif
        
        return errno == EACCES ? ERROR_PERMISSION_DENIED : ERROR_FILE_NOT_FOUND;
    }

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    fwrite(args[1].value, sizeof(char), strlen(args[1].value), file);

    fclose(file);

    return 0; // return code
}

int std_APPEND (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    FILE* file = fopen ((char*)args[0].value, "a");

    if (file == NULL) {
        #ifdef _WIN32
        _fcloseall(); // close all files if any were opened
        #endif
        return errno == EACCES ? ERROR_PERMISSION_DENIED : ERROR_FILE_NOT_FOUND;
    }

    cRes = castValue((Variable*)&args[1], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    fwrite(args[1].value, sizeof(char), strlen(args[1].value), file);

    fclose(file);

    return 0; // return code
}
#endif
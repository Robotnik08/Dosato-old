#ifndef SYSTEM_H
#define SYSTEM_H

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

int std_SYSTEM (Process* process, const Variable* args, int argc);

int std_SYSTEM (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
    if (cRes) return cRes;

    int* returnCode = malloc(sizeof(int));

    *returnCode = system((char*)args[0].value); // storing cmd code in returnCode

    
    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_INT, returnCode, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);

    return 0; // return code
}
#endif
#ifndef LISTEN_H
#define LISTEN_H

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

#include "../strtools.h"
#include "../input.h"

int std_LISTEN (Process* process, const Variable* args, int argc);

int std_LISTEN (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_STRING, 0});
        if (cRes) return cRes;
        printf("%s", (char*)args[0].value);
    }
    
    char* input = getInput();


    Variable* var = malloc(sizeof(Variable));
    *var = createVariable("-lit", TYPE_STRING, input, 0, 0);
    setReturnValue(process, var);

    destroyVariable(var);
    free(var);
    // input gets freed in destroyVariable (the variable owns the value)

    return 0; // return code
}
#endif
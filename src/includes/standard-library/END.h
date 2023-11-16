#ifndef END_H
#define END_H

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

int end (Process* process, const Variable* args, int argc);

int end (Process* process, const Variable* args, int argc) {
    if (argc == 0) {
        process->exit_code = 0;
        process->running = 0;
    } else if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_INT, 0});
        if (cRes) return cRes;
        
        process->exit_code = *(int*)args[0].value;
        process->running = 0;
    } else {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    return 0; // return code
}
#endif
#ifndef BREAK_H
#define BREAK_H

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

int std_BREAK (Process* process, const Variable* args, int argc);

int std_BREAK (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    int len = 1;
    if (argc == 1) {
        int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 0});
        if (cRes) return cRes;
        len = *(long*)args[0].value;
    }
    for (int i = 0; i < len; i++) {
        Scope* lastScope = getLastNonTerminatedScope(&process->main_scope);
        if (lastScope->callType != SCOPE_BLOCK) {
            return ERROR_BREAK_OUTSIDE_OF_LOOP;
        }
        lastScope->terminated = TERMINATE_BREAK;
    }
    return 0; // return code
}

int std_CONTINUE (Process* process, const Variable* args, int argc);

int std_CONTINUE (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    Scope* lastScope = getLastNonTerminatedScope(&process->main_scope);
    if (lastScope->callType != SCOPE_BLOCK) {
        return ERROR_CONTINUE_OUTSIDE_OF_LOOP;
    }
    lastScope->terminated = TERMINATE_CONTINUE;
    return 0; // return code
}

int std_RETURN (Process* process, const Variable* args, int argc);

int std_RETURN (Process* process, const Variable* args, int argc) {
    Scope* lastScope = NULL;

    do {
        lastScope = getLastNonTerminatedScope(&process->main_scope);
        lastScope->terminated = TERMINATE_RETURN;
    } while (lastScope->callType == SCOPE_BLOCK);

    if (lastScope->callType != SCOPE_FUNCTION && lastScope->callType != SCOPE_EXPRESSION) {
        return ERROR_RETURN_OUTSIDE_OF_FUNCTION;
    }
    lastScope->terminated = -1;

    if (lastScope->callType == SCOPE_FUNCTION) {
        if (lastScope->returnType.dataType != TYPE_VOID) {
            if (argc != 1) {
                return argc < 1 ? ERROR_TOO_FEW_ARGUMENTS : ERROR_TOO_MANY_ARGUMENTS;
            }

            int cRes = castValue((Variable*)&args[0], lastScope->returnType);
            if (cRes) return cRes;

            // set the return value
            setReturnValue(process, (Variable*)&args[0]);
        } else if (argc != 0) {
            return argc < 0 ? ERROR_TOO_FEW_ARGUMENTS : ERROR_TOO_MANY_ARGUMENTS;
        }
    } else if (lastScope->callType == SCOPE_EXPRESSION) {
        if (argc > 1) {
            return ERROR_TOO_MANY_ARGUMENTS;
        }
        if (argc) {
            // set the return value
            setReturnValue(process, (Variable*)&args[0]);
        }
    }
    return 0; // return code
}
#endif
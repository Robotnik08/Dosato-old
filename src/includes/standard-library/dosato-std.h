#ifndef DOSATO_STD_H
#define DOSATO_STD_H

#include "SAY.h" // also includes SAYLN
#include "END.h"
#include "PAUSE.h"
#include "BREAK.h" // also includes CONTINUE and RETURN
#include "LISTEN.h"
#include "CLEAR.h"
#include "SYSTEM.h"
#include "MATH.h" // includes all math functions

#include "../ast.h"
#include "../parser.h"
#include "../lexer.h"
#include "../error.h"
#include "../function.h"
#include "../variable.h"
#include "../token.h"
#include "../scope.h"
#include "../process.h"

int standard_call (Process* process, const char* name, const Variable* args, int argc);

int standard_call (Process* process, const char* name, const Variable* args, int argc) {
    if (!strcmp(name, "SAY")) {
        return std_SAY(process, args, argc, 0);
    } else if (!strcmp(name, "SAYLN")) {
        return std_SAY(process, args, argc, 1);
    } else if (!strcmp(name, "END")) {
        return std_END(process, args, argc);
    } else if (!strcmp(name, "PAUSE")) {
        return std_PAUSE(process, args, argc);
    } else if (!strcmp(name, "BREAK")) {
        return std_BREAK(process, args, argc);
    } else if (!strcmp(name, "CONTINUE")) {
        return std_CONTINUE(process, args, argc);
    } else if (!strcmp(name, "RETURN")) {
        return std_RETURN(process, args, argc);
    } else if (!strcmp(name, "LISTEN")) {
        return std_LISTEN(process, args, argc);
    } else if (!strcmp(name, "CLEAR")) {
        return std_CLEAR(process, args, argc);
    } else if (!strcmp(name, "SYSTEM")) {
        return std_SYSTEM(process, args, argc);
    }
    // math functions
    else if (!strcmp(name, "SQRT")) {
        return std_SQRT(process, args, argc);
    } else if (!strcmp (name, "POW")) {
        return std_POW(process, args, argc);
    } else if (!strcmp (name, "ROUND")) {
        return std_ROUND(process, args, argc);
    } else if (!strcmp (name, "FLOOR")) {
        return std_FLOOR(process, args, argc);
    } else if (!strcmp (name, "CEIL")) {
        return std_CEIL(process, args, argc);
    } else if (!strcmp (name, "ABS")) {
        return std_ABS(process, args, argc);
    } else if (!strcmp (name, "MIN")) {
        return std_MIN(process, args, argc);
    } else if (!strcmp (name, "MAX")) {
        return std_MAX(process, args, argc);
    } 
    // trigonometric functions
    else if (!strcmp (name, "LOG")) {
        return std_LOG(process, args, argc);
    } 
    // else if (!strcmp (name, "LOG10")) {
    //     return std_LOG10(process, args, argc);
    // } else if (!strcmp (name, "EXP")) {
    //     return std_EXP(process, args, argc);
    // } else if (!strcmp (name, "SIN")) {
    //     return std_SIN(process, args, argc);
    // } else if (!strcmp (name, "COS")) {
    //     return std_COS(process, args, argc);
    // } else if (!strcmp (name, "TAN")) {
    //     return std_TAN(process, args, argc);
    // } else if (!strcmp (name, "ASIN")) {
    //     return std_ASIN(process, args, argc);
    // } else if (!strcmp (name, "ACOS")) {
    //     return std_ACOS(process, args, argc);
    // } else if (!strcmp (name, "ATAN")) {
    //     return std_ATAN(process, args, argc);
    // } else if (!strcmp (name, "ATAN2")) {
    //     return std_ATAN2(process, args, argc);
    // }
    
    return ERROR_FUNCTION_NOT_FOUND;
}

#endif

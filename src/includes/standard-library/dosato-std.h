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
#include "RANDOM.h" // includes all random functions
#include "TIME.h" // includes all time functions
#include "FILE.h" // includes all file functions
#include "STRING.h" // includes all string functions
#include "ARRAY.h" // includes all array functions

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
    } else if (!strcmp (name, "LOG10")) {
        return std_LOG10(process, args, argc);
    } else if (!strcmp (name, "SIN")) {
        return std_SIN(process, args, argc);
    } else if (!strcmp (name, "COS")) {
        return std_COS(process, args, argc);
    } else if (!strcmp (name, "TAN")) {
        return std_TAN(process, args, argc);
    } else if (!strcmp (name, "ASIN")) {
        return std_ASIN(process, args, argc);
    } else if (!strcmp (name, "ACOS")) {
        return std_ACOS(process, args, argc);
    } else if (!strcmp (name, "ATAN")) {
        return std_ATAN(process, args, argc);
    } else if (!strcmp (name, "ATAN2")) {
        return std_ATAN2(process, args, argc);
    } else if (!strcmp (name, "EXP")) {
        return std_EXP(process, args, argc);
    }
    // random functions
    else if (!strcmp (name, "SRAND")) {
        return std_SRAND(process, args, argc);
    } else if (!strcmp (name, "RANDINT")) {
        return std_RANDINT(process, args, argc);
    } else if (!strcmp (name, "RAND")) {
        return std_RAND(process, args, argc);
    } else if (!strcmp (name, "RANDRANGE")) {
        return std_RANDRANGE(process, args, argc);
    }
    // time functions
    else if (!strcmp (name, "TIME")) {
        return std_TIME(process, args, argc);
    } else if (!strcmp (name, "DATE")) {
        return std_DATE(process, args, argc);
    } else if (!strcmp (name, "DATETIME")) {
        return std_DATETIME(process, args, argc);
    } else if (!strcmp (name, "TIMESTAMP")) {
        return std_TIMESTAMP(process, args, argc);
    } else if (!strcmp (name, "CLOCK")) {
        return std_CLOCK(process, args, argc);
    } else if (!strcmp (name, "SLEEP")) {
        return std_SLEEP(process, args, argc);
    }
    // file functions
    else if (!strcmp (name, "READ")) {
        return std_READ(process, args, argc);
    } else if (!strcmp (name, "WRITE")) {
        return std_WRITE(process, args, argc);
    } else if (!strcmp (name, "APPEND")) {
        return std_APPEND(process, args, argc);
    }
    // string functions
    else if (!strcmp (name, "SPLIT")) {
        return std_SPLIT(process, args, argc);
    } else if (!strcmp (name, "LOWERCASE")) {
        return std_LOWERCASE(process, args, argc);
    } else if (!strcmp (name, "UPPERCASE")) {
        return std_UPPERCASE(process, args, argc);
    } else if (!strcmp (name, "LENGTH")) {
        return std_LENGTH(process, args, argc);
    } else if (!strcmp (name, "SUBSTRING")) {
        return std_SUBSTRING(process, args, argc);
    } else if (!strcmp (name, "INDEXOF")) {
        return std_INDEXOF(process, args, argc);
    } else if (!strcmp (name, "LASTINDEXOF")) {
        return std_LASTINDEXOF(process, args, argc);
    } else if (!strcmp (name, "STARTSWITH")) {
        return std_STARTSWITH(process, args, argc);
    } else if (!strcmp (name, "ENDSWITH")) {
        return std_ENDSWITH(process, args, argc);
    } else if (!strcmp (name, "TRIM")) {
        return std_TRIM(process, args, argc);
    } else if (!strcmp (name, "REVERSE")) {
        return std_REVERSE(process, args, argc);
    } else if (!strcmp (name, "REPLACE")) {
        return std_REPLACE(process, args, argc);
    } else if (!strcmp (name, "CONTAINS")) {
        return std_CONTAINS(process, args, argc);
    } else if (!strcmp (name, "REMOVE")) {
        return std_REMOVE(process, args, argc);
    } else if (!strcmp (name, "INSERT")) {
        return std_INSERT(process, args, argc);
    } else if (!strcmp (name, "STRINGTOINT")) {
        return std_STRINGTOINT(process, args, argc);
    } else if (!strcmp (name, "STRINGTODOUBLE")) {
        return std_STRINGTODOUBLE(process, args, argc);
    }
    // array functions
    else if (!strcmp (name, "ARRAYSHIFT")) {
        return std_ARRAYSHIFT(process, args, argc);
    } else if (!strcmp (name, "ARRAYREMOVE")) {
        return std_ARRAYREMOVE(process, args, argc);
    } else if (!strcmp (name, "ARRAYINSERT")) {
        return std_ARRAYINSERT(process, args, argc);
    } else if (!strcmp (name, "ARRAYSLICE")) {
        return std_ARRAYSLICE(process, args, argc);
    } 
    // else if (!strcmp (name, "ARRAYFINDINDEX")) {
    //     return std_ARRAYFINDINDEX(process, args, argc);
    // } else if (!strcmp (name, "ARRAYFINDLASTINDEX")) {
    //     return std_ARRAYFINDLASTINDEX(process, args, argc);
    // } else if (!strcmp (name, "ARRAYCONTAINS")) {
    //     return std_ARRAYCONTAINS(process, args, argc);
    // }
    return ERROR_FUNCTION_NOT_FOUND;
}

#endif

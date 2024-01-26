#ifndef CLEAR_H
#define CLEAR_H

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

int std_CLEAR (Process* process, const Variable* args, int argc);

int std_CLEAR (Process* process, const Variable* args, int argc) {
    if (argc > 0) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    #ifdef _WIN32
        system("cls"); // windows
    #else
        system("clear"); // unix
    #endif
    return 0; // return code
}
#endif
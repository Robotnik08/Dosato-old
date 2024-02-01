#ifndef PAUSE_H
#define PAUSE_H

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

int std_PAUSE (Process* process, const Variable* args, int argc);

int std_PAUSE (Process* process, const Variable* args, int argc) {
    if (argc) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    printf("Press enter to continue...");
    getchar();
    return 0; // return code
}
#endif
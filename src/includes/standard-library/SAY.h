#ifndef SAY_H
#define SAY_H

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

int say (Process* process, const Variable* args);

int say (Process* process, const Variable* args) {
    return printf("%s", args[0].value) < 0;
}


#endif
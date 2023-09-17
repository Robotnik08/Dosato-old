#ifndef SCOPE_H
#define SCOPE_H

#include "variable.h"

typedef struct {
    Var* variables;
} Scope;

void destroy_scope (Scope* scp) {
    for (int i = 0; i < sizeof(scp->variables)/sizeof(Var); i++) {
        destroy_variable(&scp->variables[i]);
    }
    free(scp);
}

#endif
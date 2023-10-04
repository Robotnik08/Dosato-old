#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

#include "scope.h"
#include "str_tools.h"

typedef struct {
    Scope* scopes;
    int status;
    int reader;
} Process;

void destroy_process (Process* proc) {
    for (int i = 0; i < sizeof(proc->scopes)/sizeof(Scope); i++) {
        destroy_scope(&proc->scopes[i]);
    }
    free(proc);
}

#endif
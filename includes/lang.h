#ifndef LANG_H
#define LANG_H

#include <stdlib.h>

#include "process.h"
#include "parser.h"

const int RECURSION_LIMIT = 1000;

int run (char** code_lines, int num_lines) {
    // initialize the main Process
    Process* proc = malloc(sizeof(Process));
    proc->status = 0;	
    proc->reader = 0;

    // initialize the main Scope
    proc->scopes = malloc(sizeof(Scope)*RECURSION_LIMIT);


    while (code_lines[proc->reader] != NULL) {
        proc->reader += run_line(proc, code_lines[proc->reader]);
    }

    // free the data
    int code = proc->status;
    destroy_process(proc);

    // return the status code
    return code;
}

#endif 
/**
 * @author Sebastiaan Heins
 * @file process.h
 * @brief Contains the process struct and helper functions
 * @version 0.5
 * @date 17-10-2023
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "scope.h"
#include "garbagecollector.h"

typedef struct {
    AST* code;

    int debug;
    int running;
    int exit_code;

    int error_code;
    int error_ast_index;
    int error_location;

    Scope main_scope;
} Process;

/**
 * @brief Create a process
 * @param debug Whether or not to print debug information
 * @param main Whether or not the process is the main process
 * @return The process
 * @warning The process must be destroyed after use
*/
Process createProcess (int debug, int main);

/**
 * @brief add the global variables to the scope
 * @param scope The scope to add the variables to
 * @param main Whether or not the process is the main process
*/
void populateDefaultVariables (Scope* scope, int main);

/**
 * @brief Destroy a process, freeing all memory
 * @param process The process to destroy
*/
void destroyProcess (Process* process);

/**
 * @brief Run a process
 * @param process The process to run
 * @return The exit code of the process
*/
int runProcess (Process* process);


Process createProcess (int debug, int main) {
    Process process;
    process.code = malloc(sizeof(AST));
    addAST(&process.code, createNullTerminatedAST());

    process.main_scope = createScope(&process.code[0].root, 0, main);
    process.debug = debug;
    
    return process;
}

void destroyProcess (Process* process) {
    for (int i = 0; i < getASTsLength(process->code); i++) {
        destroyAST(&process->code[i]);
    }
    free(process->code);
    destroyScope(&process->main_scope);
}

int runProcess (Process* process) {
    process->running = 1;
    process->exit_code = 0; // the exit code of the process, defaults to 0 (success)

    while (process->running) {
        // process is prematurely stopped, when code is successfully run this is set to 1 to keep the process running
        // this is the heartbeat of the interpreter, it keeps the process running until it is stopped
        process->running = 0;


    }

    if (process->error_code != 0) {
        printError(process->code[process->error_ast_index].full_code, process->error_location, process->error_code);
    }

    return process->exit_code;
}

#endif
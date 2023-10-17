/**
 * @author Sebastiaan Heins
 * @file process.h
 * @brief Contains the process struct and helper functions
 * @version 0.0.4
 * @date 17-10-2023
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "scope.h"

typedef struct {
    AST* code;
    int debug;
    int running;
    int exit_code;
    Scope scope;
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
    process.scope = createScope(0);
    populateDefaultVariables(&process.scope, main);
    process.debug = 0;
}

void populateDefaultVariables (Scope* scope, int main) {
    // define constants in the global scope
    // ALL pointers ownership is transferred to the variable, and must be freed in the variable's destroy function

    // these variables only exist in the main process
    if (main) {
        // the return value of the last function call is stored in the _ variable
        // this variable is the only variable that can mutate it's type
        // despite it being constant, the return value of a function can modify it
        // it defaults to 0
        int* return_value = 0;
        addVariable(scope, createVariable("_", TYPE_INT, return_value, 1));

        // BOOL constants
        int* const_true = malloc(sizeof(int));
        *const_true = 1;
        int* const_false = malloc(sizeof(int));
        *const_false = 0;
        addVariable(scope, createVariable("TRUE", TYPE_BOOL, const_true, 1));
        addVariable(scope, createVariable("FALSE", TYPE_BOOL, const_false, 1));

        // MATH constants
        double* const_pi = malloc(sizeof(double));
        *const_pi = 3.14159265358979323846;
        addVariable(scope, createVariable("MATH_PI", TYPE_DOUBLE, const_pi, 1));
        double* const_e = malloc(sizeof(double));
        *const_e = 2.71828182845904523536;
        addVariable(scope, createVariable("MATH_E", TYPE_DOUBLE, const_e, 1));
    }

    // STRING constants
    char* process_name = main ? "main" : "imported";
    char* const_process_name = malloc(sizeof(char) * strlen(process_name));
    strcpy(const_process_name, process_name);
    addVariable(scope, createVariable("__PROCESS", TYPE_STRING, const_process_name, 1));
}

void destroyProcess (Process* process) {
    for (int i = 0; i < getASTsLength(process->code); i++) {
        destroyAST(&process->code[i]);
    }
    free(process->code);
    destroyScope(&process->scope);
}

int runProcess (Process* process) {
    process->running = 1;
    process->exit_code = 0; // the exit code of the process, defaults to 0 (success)

    while (process->running) {
        // process is prematurely stopped, when code is successfully run this is set to 1 to keep the process running
        // this is the heartbeat of the interpreter, it keeps the process running until it is stopped
        process->running = 0;

    
    }
    return process->exit_code;
}

#endif
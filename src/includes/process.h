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

typedef struct Process Process;

struct Process{
    AST* code;

    int debug;
    int running;
    int exit_code;

    int error_code;
    int error_ast_index;
    int error_location;

    Scope main_scope;
};


/**
 * @brief Create a process
 * @param debug Whether or not to print debug information
 * @param main Whether or not the process is the main process
 * @return The process
 * @warning The process must be destroyed after use
*/
Process createProcess (int debug, int main, AST root_ast);

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

/**
 * @brief Get the token list of an AST
 * @param process The process to get the token list from
 * @param ast_index The index of the AST to get the token list from
 * @return The token list
*/
Token* getTokenList (Process* process, int ast_index);

/**
 * @brief Get the text of a node
 * @param process The process to get the text from
 * @param node The node to get the text from
 * @param ast_index The index of the AST to get the text from
 * @return The text of the node
*/
char* getNodeText (Process* process, Node* node, int ast_index);

// include these after the struct definition to prevent circular dependencies
#include "interpreter.h"

Process createProcess (int debug, int main, AST root_ast) {
    Process process;
    process.code = malloc(sizeof(AST));
    process.code[0] = createNullTerminatedAST();
    addAST(&process.code, root_ast);

    process.error_ast_index = 0;
    process.error_location = 0;
    process.error_code = 0;

    process.main_scope = createScope(&process.code[0].root, 0, main, 0);
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
    process->error_code = 0; // the error code of the process, defaults to 0 (no error)

    while (process->running) {

        int code = next(process);

        if (code == -1) {
            // the process has finished running
            process->running = 0;
        } else if (code != 0) {
            // an error has occured
            process->running = 0;
            process->error_code = code;
            process->exit_code = code;
        }
    }

    if (process->error_code != 0) {
        printError(process->code[process->error_ast_index].full_code, process->error_location, process->error_code);
    }

    return process->exit_code;
}

Token* getTokenList (Process* process, int ast_index) {
    return process->code[ast_index].tokens;
}

char* getNodeText (Process* process, Node* node, int ast_index) {
    Token* tokens = getTokenList(process, ast_index);
    int length = tokens[node->end].end - tokens[node->start].start + 1;
    char* str = malloc(sizeof(char) * (length + 1));
    for (int i = 0; i < length; i++) {
        str[i] = process->code[ast_index].full_code[tokens[node->start].start + i];
    }
    str[length] = '\0';
    return str;
}

#endif
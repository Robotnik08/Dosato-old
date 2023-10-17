/**
 * @author Sebastiaan Heins
 * @file interpreter.h
 * @brief The main interpreter file
 * @version 0.1
 * @date 17-10-2023
*/

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "ast.h"
#include "process.h"
#include "scope.h"
#include "variable.h"
#include "log.h"

/**
 * @brief Run the interpreter, line by line
 * @param process The process to run
*/
int next (Process* process);

int next (Process* process) {
    if (process->running) {

        Scope* scope = getLastScope(&(process->main_scope));

        if (scope->running_line >= getBodyLength(&scope->body->body[scope->running_line])) {
            destroyLastScope(&process->main_scope);
            return 0; // the block has finished running
        }

        interpretCommand(process, &scope->body->body[scope->running_line]);

        scope->running_line++;
    } else {
        // the process must be running to run the next line
        process->error_code = ERROR_PROCESS_NOT_RUNNING;
        process->error_ast_index = 0;
        process->error_location = 0;
        return ERROR_PROCESS_NOT_RUNNING;
    }
}

int interpretCommand (Process* process, Node* command) {
    if (!process->running) {
        // the process must be running to run the next line
        process->error_code = ERROR_PROCESS_NOT_RUNNING;
        process->error_ast_index = 0;
        process->error_location = 0;
        return ERROR_PROCESS_NOT_RUNNING;
    }

    Scope* l_scope = getLastScope(&process->main_scope);

    switch (command->type) {
        default:
            process->error_code = ERROR_INTERPRETER_INVALID_COMMAND;
            break;
        case NODE_FUNCTION_CALL:
            printf("function call\n");
            break;
        case NODE_MAKE_VAR:
            printf("make var\n");
            break;
        case NODE_SET_VAR:
            printf("set var\n");
            break;
    }
}
#endif

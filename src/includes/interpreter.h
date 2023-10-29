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
#include "error.h"
#include "expression.h"

/**
 * @brief Run the interpreter, line by line
 * @param process The process to run
*/
int next (Process* process);

/**
 * @brief Interpret a command (node)
 * @param process The process to run
 * @param command The command to interpret
*/
int interpretCommand (Process* process, Node* command);

/**
 * @brief Interpret a function call
 * @param process The process to run
 * @param func The function to call
*/
int functionCall (Process* process, Node* func);

/**
 * @brief Interpret a variable creation
 * @param process The process to run
 * @param func The variable to create
*/
int makeVariable (Process* process, Node* func);

/**
 * @brief Interpret a variable set
 * @param process The process to run
 * @param func The variable to set
*/
int setVariable (Process* process, Node* func);

int next (Process* process) {
    if (process->running) {
        
        Scope* scope = getLastScope(&(process->main_scope));
        if (scope->running_line >= getNodeBodyLength(scope->body->body)) {
            removeLastScope(&process->main_scope);
            return -1; // the block has finished running
        }

        int code = interpretCommand(process, &scope->body->body[scope->running_line]);
        scope->running_line++;
        return code;
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
        process->error_ast_index = 0;
        process->error_location = 0;
        return ERROR_PROCESS_NOT_RUNNING;
    }

    Scope* l_scope = getLastScope(&process->main_scope);
    switch (command->type) {
        default:
            return error(process, process->error_ast_index, ERROR_INTERPRETER_INVALID_COMMAND, getTokenList(process, 0)[command->start].start);
            break;
        case NODE_FUNCTION_CALL:
            return functionCall(process, command);
            break;
        case NODE_MAKE_VAR:
            return makeVariable(process, command);
            break;
        case NODE_FUNCTION_DECLARATION:
            return 0;
            break;
        case NODE_SET_VAR:
            return setVariable(process, command);
            break;
    }
}

int functionCall (Process* process, Node* func) {
    Token* tokens = getTokenList(process, 0);
    if (func->body[0].type != NODE_FUNCTION_IDENTIFIER) {
        return error(process, process->error_ast_index, ERROR_EXPECTED_IDENTIFIER, tokens[func->start].start);
    }
    Node* func_node = func->body[0].body;
    if (func_node[0].type != NODE_IDENTIFIER) {
        return error(process, process->error_ast_index, ERROR_EXPECTED_IDENTIFIER, tokens[func_node[0].start].start);
    }
    if (func_node[1].type != NODE_ARGUMENTS) {
        return error(process, process->error_ast_index, ERROR_EXPECTED_ARGUMENTS, tokens[func_node[1].start].start);
    }
    
    char* name = getNodeText(process, &func_node[0], 0);
    int args_length = getNodeBodyLength(func_node[1].body);
    Variable* args = malloc(sizeof(Variable) * (args_length + 1));
    for (int i = 0; i < args_length; i++) {
        args[i] = createNullTerminatedVariable();
        int res = parseExpression(&args[i], process, &func_node[1].body[i]);
        if (res) return res;
    }
    args[args_length] = createNullTerminatedVariable();
    int code = callFunction(name, args, args_length, process);
    free(name);
    for (int i = 0; i < args_length; i++) {
        destroyVariable(&args[i]);
    }
    free(args);

    if (code) {
        return error(process, process->error_ast_index, code, tokens[func_node[0].start].start);
    }
    
    return code;
}

int makeVariable (Process* process, Node* line) {
    printf("make var\n");
    return 0;
}

int setVariable (Process* process, Node* line) {
    printf("set var\n");
    return 0;
}

#endif
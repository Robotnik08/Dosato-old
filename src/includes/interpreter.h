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

/**
 * @brief Interpret a function creation
 * @param process The process to run
 * @param func The function to create
*/
int makeFunction (Process* process, Node* func);

/**
 * @brief Interpret an array creation
 * @param process The process to run
 * @param func The array to create
*/
int makeArray (Process* process, Node* func);

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
        getLastScope(&process->main_scope)->running_ast = 0;
        process->error_location = 0;
        return ERROR_PROCESS_NOT_RUNNING;
    }
}

int interpretCommand (Process* process, Node* command) {
    if (!process->running) {
        // the process must be running to run the next line
        getLastScope(&process->main_scope)->running_ast = 0;
        process->error_location = 0;
        return ERROR_PROCESS_NOT_RUNNING;
    }

    Scope* l_scope = getLastScope(&process->main_scope);
    switch (command->type) {
        default:
            return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INTERPRETER_INVALID_COMMAND, getTokenStart(process, command->start));
            break;
        case NODE_FUNCTION_CALL:
            return functionCall(process, command);
            break;
        case NODE_MAKE_VAR:
            return makeVariable(process, command);
            break;
        case NODE_FUNCTION_DECLARATION:
            return makeFunction(process, command);
            break;
        case NODE_SET_VAR:
            return setVariable(process, command);
            break;
        case NODE_ARRAY_DECLARATION:
            return makeArray(process, command);
            break;
    }
}

int functionCall (Process* process, Node* func) {
    if (func->body[0].type != NODE_FUNCTION_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, func->start));
    }
    Node* func_node = func->body[0].body;
    if (func_node[0].type != NODE_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, func_node[0].start));
    }
    if (func_node[1].type != NODE_ARGUMENTS) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_ARGUMENTS, getTokenStart(process, func_node[1].start));
    }
    
    int args_length = getNodeBodyLength(func_node[1].body);
    Variable* args = malloc(sizeof(Variable) * (args_length + 1));
    for (int i = 0; i < args_length; i++) {
        args[i] = createNullTerminatedVariable();
        int res = parseExpression(&args[i], process, &func_node[1].body[i]);
        if (res) return res;
    }
    args[args_length] = createNullTerminatedVariable();
    int code = callFunction(func_node[0].text, args, args_length, process);

    for (int i = 0; i < args_length; i++) {
        destroyVariable(&args[i]);
    }
    free(args);

    if (code) {
        return error(process, getLastScope(&process->main_scope)->running_ast, code, getTokenStart(process, func_node[0].start));
    }
    
    return code;
}

int makeVariable (Process* process, Node* line) {
    if (line->body[0].type != NODE_TYPE_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_TYPE, getTokenStart(process, line->body[0].start));
    }
    if (line->body[1].type != NODE_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, line->body[1].start));
    }
    Variable var = createNullTerminatedVariable();
    int dataRes = parseExpression(&var, process, &line->body[2]);
    if (dataRes) return dataRes;
    var.name = malloc(sizeof(char) * (strlen(line->body[1].text) + 1));
    strcpy(var.name, line->body[1].text);
    var.constant = 0;
    Type t;
    t.dataType = getTokenAtPosition(process, line->body[0].start).carry;
    t.array = 0;
    int castRes = castValue(&var, t);
    if (castRes) return ERROR_TYPE_MISMATCH;
    addVariable(getLastScope(&process->main_scope), var);
    return 0;
}

int setVariable (Process* process, Node* line) {
    
    OperatorType operator = getTokenAtPosition(process, line->body[1].start).carry;
    if (!isAssignmentOperator(operator) || line->body[1].type != NODE_OPERATOR) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_ASSIGN_OPERATOR, getTokenStart(process, line->body[1].start));
    }
    
    Variable* left;
    int left_res = parseRefrenceExpression(&left, process, &line->body[0]); // we need to retrieve the refrerence, so we overwrite the variable
    if (left_res) return left_res;
    if (left->constant) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_CANNOT_MODIFY_CONSTANT, getTokenStart(process, line->body[0].start));
    }
    Variable* right = malloc(sizeof(Variable)); // right does need to be freed, it's a value type
    *right = createNullTerminatedVariable();
    int rightRes = parseExpression(right, process, &line->body[2]); // we need to retrieve the value
    if (rightRes) return rightRes;
    
    int setRest = setVariableValue(left, right, operator);
    if (setRest) return error(process, getLastScope(&process->main_scope)->running_ast, setRest, getTokenStart(process, line->body[2].start));
    if (!strcmp(right->name, "-lit")) {
        destroyVariable(right);
        free(right);
    }
    return 0;
}

int makeFunction (Process* process, Node* line) {
    printf("make function\n");
    return 0;
}

int makeArray (Process* process, Node* line) {
    Type t = (Type){D_NULL,0};
    int array_depth = 0;
    Node* end_node = line;
    while (getTokenAtPosition(process, end_node->body[0].start).carry == TYPE_ARRAY) {
        t.array++;
        end_node = &end_node->body[1];
    }
    t.dataType = getTokenAtPosition(process, end_node->body[0].start).carry;

    Variable var = createNullTerminatedVariable();
    int dataRes = parseExpression(&var, process, &end_node->body[2]);
    if (dataRes) return dataRes;
    if (!compareType(var.type, t)) {
        int cRes = castValue(&var, t);
        if (cRes) return error(process, getLastScope(&process->main_scope)->running_ast, cRes, getTokenStart(process, end_node->body[0].start));
    }
    var.name = malloc(sizeof(char) * (strlen(end_node->body[1].text) + 1));
    strcpy(var.name, end_node->body[1].text);
    var.constant = 0;

    addVariable(getLastScope(&process->main_scope), var);
    return 0;
}

#endif
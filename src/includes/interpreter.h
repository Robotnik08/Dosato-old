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
int functionCall (Process* process, Node* func, int start);

/**
 * @brief Interpret a function call chain
 * @param process The process to run
 * @param func The function to call
 * @param start The start of the call chain
 * @param end The end of the call chain
 * @return The exit code of the function call chain
*/
int parseCallChain (Process* process, Node* func, int start, int end);

/**
 * @brief Interpret a function call
 * @param process The process to run
 * @param func The function to call
*/
int parseCall (Process* process, Node* call);

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
        TerminateType term = scope->terminated;
        if (scope->running_line >= getNodeBodyLength(scope->body->body) || term) {
            removeLastScope(&process->main_scope);
            return term ? term : -1; // the block has finished running
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

    switch (command->type) {
        default:
            return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INTERPRETER_INVALID_COMMAND, getTokenStart(process, command->start));
            break;
        case NODE_FUNCTION_CALL:
            return functionCall(process, command, 0);
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

int functionCall (Process* process, Node* func, int start) {
    if (func->body == NULL) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, func->start));
    }
    if (func->body[start].type != NODE_FUNCTION_IDENTIFIER && func->body[start].type != NODE_BLOCK) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, func->body[start].start));
    }

    // get any WHEN or WHILE statements, these encompass everything before the function call
    int extension_length = getNodeBodyLength(func->body);
    int condition_location = -1;
    int loop = 0;
    for (int i = start; i < extension_length; i++) {
        if (func->body[i].type == NODE_WHEN || func->body[i].type == NODE_WHILE) {
            condition_location = i + 1;
            loop = func->body[i].type == NODE_WHILE;
            if (func->body[i].type == NODE_WHILE && i + 1 != extension_length-1) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_WHILE_NOT_LAST, getTokenStart(process, func->body[i].start));
            }
            if (func->body[i].type == NODE_WHEN) {
                if (i+1 == extension_length-1) break;
                if (func->body[i+2].type != NODE_ELSE) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_ELSE, getTokenStart(process, func->body[i].start));
                }
            }
            break;
        }
    }
    if (!loop) {
        // when the condition is not a loop, we need to check if the condition is true
        if (condition_location != -1) {
            Variable* condition = malloc(sizeof(Variable));
            *condition = createNullTerminatedVariable();
            int condition_res = parseExpression(condition, process, &func->body[condition_location]);
            if (condition_res) return condition_res;
            int cast_res = castValue(condition, (Type){TYPE_BOOL,0});
            if (cast_res) return error(process, getLastScope(&process->main_scope)->running_ast, cast_res, getTokenStart(process, func->body[condition_location].start));
            if (!*(int*)condition->value) {
                if (condition_location == extension_length-1) return 0;
                return functionCall(process, func, condition_location+2);
            }

            if (!strcmp(condition->name, "-lit")) {
                destroyVariable(condition);
                free(condition);
            }
        }

        // parse all the expressions before the WHEN when the condition is true
        return parseCallChain(process, func, start, condition_location == -1 ? extension_length : condition_location-1);
    }
    while (1) {
        Variable* condition = malloc(sizeof(Variable));
        *condition = createNullTerminatedVariable();
        int condition_res = parseExpression(condition, process, &func->body[condition_location]);
        if (condition_res) return condition_res;
        int cast_res = castValue(condition, (Type){TYPE_BOOL,0});
        if (cast_res) return error(process, getLastScope(&process->main_scope)->running_ast, cast_res, getTokenStart(process, func->body[condition_location].start));
        int condition_result = *(int*)condition->value;
        if (!strcmp(condition->name, "-lit")) {
            destroyVariable(condition);
            free(condition);
        }
        
        if (!condition_result) { // if the condition is false, we stop the loop
            return 0;
        }

        int call_res = parseCallChain(process, func, start, condition_location-1);
        if (call_res > 0) return call_res;

        if (call_res == TERMINATE_BREAK || call_res == TERMINATE_RETURN) {
            return 0;
        }
    }
    return 0;
}

int parseCallChain (Process* process, Node* func, int start, int end) {
    if (start > end) return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_IDENTIFIER_INVALID, getTokenStart(process, func->start));
    
    // the first call of a call chain is always a function call
    int call_res = parseCall(process, &func->body[start]);

    // check if the next extension is a THEN, CATCH or INTO (CATCH and INTO only are at the end of the call chain)
    for (int i = start+1; i < end; i+= 2) { // skipping 2 per, since It's: EXT, CALL, EXT, CALL
        switch (func->body[i].type) {
            case NODE_THEN:
                // if the call was successful, run the next call
                if (call_res == 0) {
                    call_res = parseCall(process, &func->body[i+1]);
                }
                break;
            case NODE_CATCH:
            case NODE_INTO:
                // if this is not the last call, we return an error
                if (i+2 != end) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXTENSION_NOT_FINAL, getTokenStart(process, func->body[i].start));
                }
                break;
        }
        if (call_res > 0) break; // if the call was not successful, we stop the call chain and perhapse run a CATCH
    }
    if (call_res > 0) {
        if (func->body[end-2].type == NODE_CATCH) {
            Variable* underscore = getVariable(&process->main_scope, "_"); // we store the error in the _ variable
            if (underscore == NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INTERNAL, getTokenStart(process, func->body[end-1].start));
            }

            // set the _ variable to the error
            destroyVariable(underscore);
            *underscore = createNullTerminatedVariable();
            underscore->name = malloc(sizeof(char) * 2);
            strcpy(underscore->name, "_");
            underscore->constant = 1;
            underscore->type = (Type){TYPE_INT,0};
            int* error = malloc(sizeof(int));
            *error = call_res;
            underscore->value = error;

            // if the catch block was successful, we reset the error state of the process
            process->error_code = 0;
            process->error_ast_index = 0;
            process->error_location = 0;
            process->running = 1; // we set the process to running again

            // run the catch block
            int catch_res = parseCall(process, &func->body[end-1]);
            return catch_res;
        }
        return call_res; // if theres no catch, throw the error
    }
    if (func->body[end-2].type == NODE_INTO) {
        Variable* left;
        int left_res = parseRefrenceExpression(&left, process, &func->body[end-1]);
        if (left_res) return left_res;

        Variable* underscore = getVariable(&process->main_scope, "_"); // we store _ into the provided variable
        if (underscore == NULL) {
            return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INTERNAL, getTokenStart(process, func->body[end-1].start));
        }

        // set the provided variable to the _ variable
        int setRes = setVariableValue(left, underscore, OPERATOR_ASSIGN);
        if (setRes) return error(process, getLastScope(&process->main_scope)->running_ast, setRes, getTokenStart(process, func->body[end-1].start));
    }
    return call_res;
}

int parseCall (Process* process, Node* call) {
    // parse block as inline function
    if (call->type == NODE_BLOCK) {
        // create a new scope to run the function in
        Scope scope = createScope(call, getLastScope(&process->main_scope)->running_ast, 0, getScopeLength(&process->main_scope), SCOPE_BLOCK);
        *getLastScope(&process->main_scope)->child = scope;

        // excute the function in here
        int code = 0;
        while (process->running) {
            code = next(process);
            if (code) break;
        }
        if (code == -1) code = 0;
        return code;
    }

    // parse function call to existing function
    Node* func_node = call->body;
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
    if (code && process->running) {
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

    // check if variable already exists
    if (getVariable(getLastScope(&process->main_scope), line->body[1].text) != NULL) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_VARIABLE_ALREADY_EXISTS, getTokenStart(process, line->body[1].start));
    }

    Variable var = createNullTerminatedVariable();
    int dataRes = parseExpression(&var, process, &line->body[2]);
    if (dataRes) return dataRes;
    free (var.name); // free the old name, since we're going to overwrite it
    var.name = malloc(sizeof(char) * (strlen(line->body[1].text) + 1));
    strcpy(var.name, line->body[1].text);
    var.constant = 0;
    Type t;
    t.dataType = getTokenAtPosition(process, line->body[0].start).carry;
    t.array = 0;
    int castRes = castValue(&var, t);
    if (castRes) return error(process, getLastScope(&process->main_scope)->running_ast, castRes, getTokenStart(process, line->body[2].start));
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
    
    int setRes = setVariableValue(left, right, operator);
    if (setRes) return error(process, getLastScope(&process->main_scope)->running_ast, setRes, getTokenStart(process, line->body[2].start));

    if (!strcmp(right->name, "-lit")) {
        destroyVariable(right);
        free(right);
    }
    return 0;
}

int makeFunction (Process* process, Node* line) {
    if (line->body[0].type != NODE_TYPE_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_TYPE, getTokenStart(process, line->body[0].start));
    }

    if (line->body[1].type != NODE_IDENTIFIER) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, line->body[1].start));
    }

    if (line->body[2].type != NODE_FUNCTION_DECLARATION_ARGUMENTS) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_ARGUMENTS, getTokenStart(process, line->body[2].start));
    }

    if (line->body[3].type != NODE_BLOCK) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_BLOCK, getTokenStart(process, line->body[3].start));
    }

    // check if function already exists
    if (getFunction(&process->main_scope, line->body[1].text) != NULL) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_FUNCTION_ALREADY_EXISTS, getTokenStart(process, line->body[1].start));
    }

    // make arguments
    int args_length = getNodeBodyLength(line->body[2].body);
    Argument* args = malloc(sizeof(Argument) * (args_length + 1));
    for (int i = 0; i < args_length; i++) {
        Type t = (Type){D_NULL,0};
        int array_depth = 0;
        Node* end_node = &line->body[2].body[i];
        while (getTokenAtPosition(process, end_node->body[0].start).carry == TYPE_ARRAY) {
            t.array++;
            end_node = &end_node->body[1];
        }
        t.dataType = getTokenAtPosition(process, end_node->body[0].start).carry;

        if (end_node->body[1].type != NODE_IDENTIFIER) {
            return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_IDENTIFIER, getTokenStart(process, end_node->body[1].start));
        }
        args[i] = createArgument(end_node->body[1].text, t);
    }


    Type returnType = (Type){.dataType = D_NULL, .array = 0};
    int tRes = getTypeFromNode(process, &returnType, &line->body[0]);
    if (tRes) return error(process, getLastScope(&process->main_scope)->running_ast, tRes, getTokenStart(process, line->body[0].start));

    addFunction(&process->main_scope, createFunction(line->body[1].text, &line->body[3], args, args_length, returnType, 0));

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
    if (t.array == 0) return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_EXPECTED_ARRAY, getTokenStart(process, line->body[0].start));
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
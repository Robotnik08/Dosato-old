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
 * @brief Set the return value of a process
 * @param process The process to set the return value of
 * @param var The variable to set the return value to
*/
void setReturnValue (Process* process, Variable* var);

/**
 * @brief Get the return value of a process
 * @param process The process to get the return value from
 * @return The return value
*/
Variable* getReturnValue (Process* process);

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
 * @return The text of the node (must be freed after use)
*/
char* getNodeText (Process* process, Node* node, int ast_index);

/**
 * @brief Call a function
 * @param name The name of the function to call
 * @param args The arguments to pass to the function
 * @param args_length The length of the arguments
 * @param process The process to call the function in
 * @return The error code
*/
int callFunction (char* name, Variable* args, int args_length, Process* process);

/**
 * @brief Get the position in the full code of a tokens start
 * @param process The process to get the token from
 * @param token The index of the token
 * @return The position in the full code of the tokens start
*/
int getTokenStart (Process* process, int token);

/**
 * @brief Get the position in the full code of a tokens end
 * @param process The process to get the token from
 * @param token The index of the token
 * @return The position in the full code of the tokens end
*/
int getTokenEnd (Process* process, int token);

/**
 * @brief Get the token at a position in the current running AST
 * @param process The process to get the token from
 * @param position The position in the full code
 * @return The token at the position
*/
Token getTokenAtPosition (Process* process, int position);

/**
 * @brief Get the type from a cast node
 * @param t The type to fill
 * @param cast The cast node
 * @return The error code
*/
int getTypeFromCastNode (Process* process, Type* t, Node* cast);

/**
 * @brief Get the type from a type node
 * @param t The type to fill
 * @param typeNode The type node
 * @return The error code
*/
int getTypeFromNode (Process* process, Type* t, Node* typeNode);

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
    process.exit_code = 0;
    process.running = 0;

    process.main_scope = createScope(&process.code[0].root, 0, main, 0, SCOPE_ROOT);
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
        destroyProcess(process);
    }

    return process->exit_code;
}

void setReturnValue (Process* process, Variable* var) {
    Variable* returnVariable = getVariable(&process->main_scope, "_");

    destroyVariable(returnVariable);

    *returnVariable = cloneVariable(var);
    free (returnVariable->name);
    returnVariable->name = malloc(sizeof(char) * 2);
    strcpy(returnVariable->name, "_");
    returnVariable->constant = 1;
}

Variable* getReturnValue (Process* process) {
    return getVariable(&process->main_scope, "_");
}

Token* getTokenList (Process* process, int ast_index) {
    return process->code[ast_index].tokens;
}

char* getNodeText (Process* process, Node* node, int ast_index) {
    if (node->end < node->start) {
        return "error, node end is before node start";
    }
    Token* tokens = getTokenList(process, ast_index);
    int length = tokens[node->end].end - tokens[node->start].start + 1;
    char* str = malloc(sizeof(char) * (length + 1));
    for (int i = 0; i < length; i++) {
        str[i] = process->code[ast_index].full_code[tokens[node->start].start + i];
    }
    str[length] = '\0';
    return str;
}

int getTokenStart (Process* process, int token) {
    return process->code[getLastScope(&process->main_scope)->running_ast].tokens[token].start;
}

int getTokenEnd (Process* process, int token) {
    return process->code[getLastScope(&process->main_scope)->running_ast].tokens[token].end;
}

Token getTokenAtPosition (Process* process, int position) {
    return process->code[getLastScope(&process->main_scope)->running_ast].tokens[position];
}

int getTypeFromCastNode (Process* process, Type* t, Node* cast) {
    if (cast->type != NODE_OPERATOR_CAST) return ERROR_INVALID_CAST;
    if (cast->start + 1 >= cast->end) return ERROR_INVALID_CAST;

    for (int i = cast->start + 1; i < cast->end-1; i++) {
        if (getTokenAtPosition(process, i).carry == TYPE_ARRAY) {
            t->array++;
        } else {
            return ERROR_INVALID_CAST;
        }
    }

    if (getTokenAtPosition(process, cast->end-1).carry == TYPE_ARRAY) {
        return ERROR_INVALID_CAST;
    } else {
        t->dataType = getTokenAtPosition(process, cast->end-1).carry;
    }
    
    return 0;
}

int getTypeFromNode (Process* process, Type* t, Node* typeNode) {
    if (typeNode->type != NODE_TYPE_IDENTIFIER) return ERROR_INVALID_TYPE;
    if (typeNode->start > typeNode->end) return ERROR_INVALID_TYPE;

    for (int i = typeNode->start; i < typeNode->end; i++) {
        if (getTokenAtPosition(process, i).carry == TYPE_ARRAY) {
            t->array++;
        } else {
            return ERROR_INVALID_CAST;
        }
    }

    if (getTokenAtPosition(process, typeNode->end).carry == TYPE_ARRAY) {
        return ERROR_INVALID_CAST;
    } else {
        t->dataType = getTokenAtPosition(process, typeNode->end).carry;
    }
    
    return 0;
}
#include "standard-library/dosato-std.h" // include the dosato standard library, after all the other definitions

int callFunction (char* name, Variable* args, int args_length, Process* process) {
    Function* function = getFunction(&process->main_scope, name);
    if (function == NULL) {
        return ERROR_FUNCTION_NOT_FOUND;
    }
    
    if (function->std_function) {
        return standard_call(process, name, args, args_length);
    }

    if (function->arguments_length != args_length) {
        return ERROR_FUNCTION_ARG_NOT_CORRECT_AMOUNT;
    }

    // create a new scope to run the function in
    Scope scope = createScope(function->body, getLastScope(&process->main_scope)->running_ast, 0, getScopeLength(&process->main_scope), SCOPE_FUNCTION);
    scope.returnType = function->return_type;
    
    // add the arguments to the scope
    for (int i = 0; i < args_length; i++) {
        Variable arg = cloneVariable(&args[i]); // clone the variable, so we can change the name
        free(arg.name);
        arg.name = malloc(sizeof(char) * (strlen(function->arguments[i].name) + 1));
        strcpy(arg.name, function->arguments[i].name);
        if (!compareType(function->arguments[i].type, arg.type)) {
            int cRes = castValue(&arg, function->arguments[i].type);
            if (cRes) return cRes;
        }
        addVariable(&scope, arg);
    }

    *getLastScope(&process->main_scope)->child = scope;

    // excute the function in here
    int code = 0;
    while (process->running) {
        code = next(process);
        if (code) break;
    }
    return code > 0 ? code : 0;
}

#endif
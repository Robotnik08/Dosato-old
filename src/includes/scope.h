/**
 * @author Sebastiaan Heins
 * @file scope.h
 * @brief Contains the scope struct and helper functions
 * @version 0.5
 * @date 17-10-2023
*/

#ifndef SCOPE_H
#define SCOPE_H

#include <stdio.h>

#include "token.h"
#include "variable.h"
#include "function.h"

typedef struct Scope Scope;

typedef enum {
    SCOPE_ROOT,
    SCOPE_BLOCK,
    SCOPE_FUNCTION,
    SCOPE_EXPRESSION
} ScopeType;

typedef enum {
    TERMINATE_BREAK = -2,
    TERMINATE_CONTINUE = -3,
    TERMINATE_RETURN = -4
} TerminateType;

struct Scope {
    int running_line;
    int running_ast;
    Variable* variables;
    Function* functions;
    Node* body;
    Scope* child;

    TerminateType terminated;

    Type returnType;
    ScopeType callType;
};

#include "garbagecollector.h"


/**
 * @brief Populate the default variables in a scope
 * @param scope The scope to add the variables to
 * @param main Whether or not the process is the main process
*/
void populateDefaultVariables (Scope* scope, int main, int depth);

/**
 * @brief Create a scope
 * @param running_line The line the scope is running on
 * @return The scope
 * @warning The scope must be destroyed after use
*/
Scope createScope (Node* body, int ast_index, int main, int depth, ScopeType callType);

/**
 * @brief Create a null terminated scope
 * @return The null terminated scope
*/
Scope createNullTerminatedScope ();

/**
 * @brief Get the length of a scope chain
 * @param scope The scope to get the length of
 * @return The length of the scope chain
*/
int getScopeLength (const Scope* scope);

/**
 * @brief Destroy a scope, freeing all memory
 * @param scope The scope to destroy
*/
void destroyScope (Scope* scope);

/**
 * @brief Get the last scope in a scope chain
 * @param scope The scope to get the last scope of
 * @return The last scope
*/
Scope* getLastScope (Scope* scope);

/**
 * @brief Get the last non terminated scope in a scope chain
 * @param scope The scope to get the last scope of
 * @return The last scope
*/
Scope* getLastNonTerminatedScope (Scope* scope);

/**
 * @brief Remove the last scope in a scope chain
 * @param scope The scope to remove the last scope of
*/
void removeLastScope (Scope* scope);

/**
 * @brief Add a variable to a scope
 * @param scope The scope to add the variable to
 * @param variable The variable to add
*/
void addVariable (Scope* scope, Variable variable);

/**
 * @brief Add a function to a scope
 * @param scope The scope to add the function to
 * @param variable The function to add
*/
void addFunction (Scope* scope, Function func);

/**
 * @brief Get a variable from a scope
 * @param scope The scope to get the variable from
 * @param name The name of the variable
*/
Variable* getVariable (Scope* scope, char* name);

/**
 * @brief Get a variable from a list of variables
 * @param list The list of variables to get the variable from
 * @param name The name of the variable
*/
Variable* getVariableFromList (Variable* list, char* name);

/**
 * @brief Get a function from a scope
 * @param scope The scope to get the function from
 * @param name The name of the function
*/
Function* getFunction (Scope* scope, char* name);

/**
 * @brief Add a scope to a scope array
 * @param scope The scope to add the variable to
 * @param variable The variable to add to the scope
*/
void addScope (Scope** scope, Scope new_scope);

void populateDefaultVariables (Scope* scope, int main, int depth) {
    // define constants in the global scope
    // ALL pointers ownership is transferred to the variable, and must be freed in the variable's destroy function

    // these variables only exist in the main process
    if (main) {
        // the return value of the last function call is stored in the _ variable
        // this variable is the only variable that can mutate it's type
        // despite it being constant, the return value of a function can modify it
        // it defaults to 0
        int* return_value = malloc(sizeof(int));
        *return_value = 0;
        addVariable(scope, createVariable("_", TYPE_INT, return_value, 1, 0));

        // BOOL constants
        int* const_true  = malloc(sizeof(int));
        *const_true = 1;
        int* const_false = malloc(sizeof(int));
        *const_false = 0;
        addVariable(scope, createVariable("TRUE", TYPE_BOOL, const_true, 1, 0));
        addVariable(scope, createVariable("FALSE", TYPE_BOOL, const_false, 1, 0));

        // MATH constants
        double* const_pi = malloc(sizeof(double));
        *const_pi = 3.14159265358979323846;
        addVariable(scope, createVariable("MATH_PI", TYPE_DOUBLE, const_pi, 1, 0));
        double* const_e = malloc(sizeof(double));
        *const_e = 2.71828182845904523536;
        addVariable(scope, createVariable("MATH_E", TYPE_DOUBLE, const_e, 1, 0));
    }

    // the depth of the scope is stored in the __depth variable
    int* const_scope_depth = malloc(sizeof(int));
    *const_scope_depth = depth;
    addVariable(scope, createVariable("__depth", TYPE_INT, const_scope_depth, 1, 0));
}

void addSystemFunctions (Scope* scope, int main, int depth) {
    if (main) {
        // SAY function
        addFunction(scope, createFunction("SAY", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // SAYLN function
        addFunction(scope, createFunction("SAYLN", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // END function
        addFunction(scope, createFunction("END", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // PAUSE function
        addFunction(scope, createFunction("PAUSE", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // BREAK function
        addFunction(scope, createFunction("BREAK", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // CONTINUE function
        addFunction(scope, createFunction("CONTINUE", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // RETURN function
        addFunction(scope, createFunction("RETURN", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // LISTEN function
        addFunction(scope, createFunction("LISTEN", NULL, NULL, 0, (Type){TYPE_STRING, 0}, 1));

        // CLEAR function
        addFunction(scope, createFunction("CLEAR", NULL, NULL, 0, (Type){TYPE_VOID, 0}, 1));

        // SYSTEM function
        addFunction(scope, createFunction("SYSTEM", NULL, NULL, 0, (Type){TYPE_INT, 0}, 1));

        // MATH functions

        // SQRT function
        addFunction(scope, createFunction("SQRT", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // POW function
        addFunction(scope, createFunction("POW", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // ROUND function
        addFunction(scope, createFunction("ROUND", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // FLOOR function
        addFunction(scope, createFunction("FLOOR", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // CEIL function
        addFunction(scope, createFunction("CEIL", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // ABS function
        addFunction(scope, createFunction("ABS", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // MIN function
        addFunction(scope, createFunction("MIN", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // MAX function
        addFunction(scope, createFunction("MAX", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // LOG function
        addFunction(scope, createFunction("LOG", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));

        // LOG10 function
        addFunction(scope, createFunction("LOG10", NULL, NULL, 0, (Type){TYPE_DOUBLE, 0}, 1));
    }
}

Scope createScope (Node* body, int ast_index, int main, int depth, ScopeType callType) {
    Scope scope;
    scope.body = body;
    scope.running_line = 0;
    scope.terminated = 0;
    scope.running_ast = ast_index;

    scope.returnType = (Type){TYPE_VOID, 0};
    scope.callType = callType;
    
    scope.variables = malloc(sizeof(Variable));
    scope.variables[0] = createNullTerminatedVariable();
    populateDefaultVariables(&scope, main, depth);

    scope.functions = malloc(sizeof(Function));
    scope.functions[0] = createNullTerminatedFunction();
    addSystemFunctions(&scope, main, depth);

    Scope* child = malloc(sizeof(Scope));
    *child = createNullTerminatedScope();
    scope.child = child;
    return scope;
}

Scope createNullTerminatedScope () {
    Scope scope;
    scope.body = NULL;
    scope.running_line = -1;
    scope.variables = NULL;
    scope.child = NULL;

    scope.returnType = (Type){TYPE_VOID, 0};
    scope.callType = SCOPE_ROOT;
    scope.terminated = 0;
    return scope;
}

int getScopeLength (const Scope* scope) {
    int length = 0;
    while (scope->running_line != -1) {
        length++;
        scope = scope->child;
    }
    return length;
}

void destroyScope (Scope* scope) {
    cleanScope(scope);
}

Scope* getLastScope (Scope* scope) {
    Scope* last_scope = scope;
    while (last_scope->child->running_line != -1) {
        last_scope = last_scope->child;
    }
    return last_scope;
}

Scope* getLastNonTerminatedScope (Scope* scope) {
    Scope* last_scope = scope;
    while (last_scope->child->running_line != -1 && !last_scope->child->terminated) {
        last_scope = last_scope->child;
    }
    return last_scope;
}

void removeLastScope (Scope* scope) {
    Scope* last_scope = scope;
    if (getLastScope(scope) == scope) {
        // there is only one scope, this scope cannot be removed
        return;
    }

    while (last_scope->child->child != NULL) {
        last_scope = last_scope->child;
    }
    destroyScope(last_scope);
    *last_scope = createNullTerminatedScope();
}

void addVariable (Scope* scope, Variable variable) {
    int length = getVariablesLength(scope->variables);
    scope->variables = realloc(scope->variables, sizeof(Variable) * (length + 2));
    scope->variables[length] = variable;
    scope->variables[length+1] = createNullTerminatedVariable();
}

void addFunction (Scope* scope, Function func) {
    int length = getFunctionsLength(scope->functions);
    scope->functions = realloc(scope->functions, sizeof(Function) * (length + 2));
    scope->functions[length] = func;
    scope->functions[length+1] = createNullTerminatedFunction();
}

Variable* getVariableFromList (Variable* list, char* name) {
    int length = getVariablesLength(list);
    for (int i = 0; i < length; i++) {
        if (!strcmp(list[i].name, name)) {
            return &list[i];
        }
    }
    return NULL;
}

Variable* getVariable (Scope* scope, char* name) {
    Variable* ret = NULL;
    while (scope->running_line != -1) {
        Variable* variable = getVariableFromList(scope->variables, name);
        if (variable != NULL) {
            ret = variable;
        }
        scope = scope->child;
    }
    return ret;
}

Function* getFunction (Scope* scope, char* name) {
    int length = getFunctionsLength(scope->functions);
    for (int i = 0; i < length; i++) {
        if (!strcmp(scope->functions[i].name, name)) {
            return &scope->functions[i];
        }
    }
    return NULL;
}

void addScope (Scope** scope, Scope new_scope) {
    int length = getScopeLength(*scope);
    *scope = realloc(*scope, sizeof(Scope) * (length + 2));
    (*scope)[length] = new_scope;
    (*scope)[length+1] = createNullTerminatedScope();
}

#endif
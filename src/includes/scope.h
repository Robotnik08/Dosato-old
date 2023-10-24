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

struct Scope {
    int running_line;
    int running_ast;
    Variable* variables;
    Function* functions;
    Node* body;
    Scope* child;
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
Scope createScope (Node* body, int ast_index, int main, int depth);

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
        int* return_value = 0;
        addVariable(scope, createVariable("_", TYPE_INT, return_value, 1, 0));

        // BOOL constants
        int* const_true = malloc(sizeof(int));
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

void addSystemVariables (Scope* scope, int main, int depth) {
    if (main) {
        
    }
}

Scope createScope (Node* body, int ast_index, int main, int depth) {
    Scope scope;
    scope.body = body;
    scope.running_line = 0;
    scope.running_ast = ast_index;
    scope.variables = malloc(sizeof(Variable));
    scope.variables[0] = createNullTerminatedVariable();
    populateDefaultVariables(&scope, main, depth);

    scope.functions = malloc(sizeof(Function));
    scope.functions[0] = createNullTerminatedFunction();

    scope.child = NULL;
    return scope;
}

Scope createNullTerminatedScope () {
    Scope scope;
    scope.body = NULL;
    scope.running_line = -1;
    scope.variables = NULL;
    scope.child = NULL;
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
    while (last_scope->child != NULL) {
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
    destroyScope(last_scope->child);
    last_scope->child = NULL;
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

void addScope (Scope** scope, Scope new_scope) {
    int length = getScopeLength(*scope);
    *scope = realloc(*scope, sizeof(Scope) * (length + 2));
    (*scope)[length] = new_scope;
    (*scope)[length+1] = createNullTerminatedScope();
}

#endif
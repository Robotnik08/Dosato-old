/**
 * @author Sebastiaan Heins
 * @file scope.h
 * @brief Contains the scope struct and helper functions and the variable struct and helper functions
 * @version 0.0.4
 * @date 17-10-2023
*/

#ifndef SCOPE_H
#define SCOPE_H

#include <stdio.h>

#include "token.h"

typedef struct {
    char* name;
    DataType type;
    void* value;
    int constant;
} Variable;

typedef struct Scope Scope;

struct Scope {
    int running_line;
    Variable* variables;
    Scope* child;
};

/**
 * @brief Create a variable
 * @param name The name of the variable
 * @param type The type of the variable
 * @param valueptr The pointer to the value of the variable
 * @param constant Whether or not the variable is constant
 * @return The variable
*/
Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant);

/**
 * @brief Create a null terminated variable
 * @return The null terminated variable
*/
Variable createNullTerminatedVariable ();

/**
 * @brief Get the length of a list of variables
 * @param list The list of variables
 * @return The length of the list
*/
int getVariablesLength (const Variable* list);

/**
 * @brief Create a scope
 * @param running_line The line the scope is running on
 * @return The scope
 * @warning The scope must be destroyed after use
*/
Scope createScope (const int running_line);

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
 * @brief Add a variable to a scope
 * @param scope The scope to add the variable to
 * @param variable The variable to add
*/
void addVariable (Scope* scope, Variable variable);


Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant) {
    Variable variable;
    variable.name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(variable.name, name);
    variable.type = type;
    variable.value = valueptr;
    variable.constant = constant;
    return variable;
}

Variable createNullTerminatedVariable () {
    Variable variable;
    variable.name = NULL;
    return variable;
}

int getVariablesLength (const Variable* list) {
    int length = 0;
    while (list[length].name != NULL) {
        length++;
    }
    return length;
}

Scope createScope (const int running_line) {
    Scope scope;
    scope.running_line = running_line;
    scope.variables = malloc(sizeof(Variable));
    scope.variables[0] = createNullTerminatedVariable();
    scope.child = NULL;
    return scope;
}

void destroyScope (Scope* scope) {
    // for now we don't need to do anything
}

Scope* getLastScope (Scope* scope) {
    Scope* last_scope = scope;
    while (last_scope->child != NULL) {
        last_scope = last_scope->child;
    }
    return last_scope;
}

void addVariable (Scope* scope, Variable variable) {
    int length = getVariablesLength(scope->variables);
    scope->variables = realloc(scope->variables, sizeof(Variable) * (length + 2));
    scope->variables[length] = variable;
    scope->variables[length+1] = createNullTerminatedVariable();
}

#endif
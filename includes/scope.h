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

Variable createVariable (const char* name, const DataType type, void* valueptr, const int constant);
Variable createNullTerminatedVariable ();
int getVariablesLength (const Variable* list);
Scope createScope (const int running_line);
void destroyScope (Scope* scope);
Scope* getLastScope (Scope* scope);
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
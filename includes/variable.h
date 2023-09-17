#ifndef VARIABLE_H
#define VARIABLE_H

typedef struct {
    char* name;
    int* value;
} Var;

void destroy_variable (Var* var) {
    free(var->name);
    free(var->value);
    free(var);
}

#endif
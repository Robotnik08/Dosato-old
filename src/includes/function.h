#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "ast.h"
#include "strtools.h"
#include "log.h"
#include "process.h"
#include "variable.h"

typedef struct Function Function;

typedef struct Argument Argument;

struct Argument {
    char* name;
    DataType type;
};

struct Function {
    char* name;

    Node* body;
    int arguments_length;
    Argument* arguments;

    DataType return_type;
    
    int std_function;
};

/**
 * @brief Create a function
 * @param name The name of the function
 * @param body The body of the function
 * @param arguments The arguments of the function
 * @param arguments_length The length of the arguments
 * @return The function
 */
Function createFunction (char* name, Node* body, Argument* arguments, int arguments_length);

/**
 * @brief Create a null terminated function
 * @return The null terminated function
 */
Function createNullTerminatedFunction ();

/**
 * @brief Get the length of a list of functions
 * @param list The list of functions
 * @return The length of the list
 */
int getFunctionsLength (const Function* list);

/**
 * @brief Destroy a function, freeing all memory
 * @param function The function to destroy
 */
void destroyFunction (Function* function);


Function createFunction (char* name, Node* body, Argument* arguments, int arguments_length) {
    Function function;
    function.name = name;
    function.body = body;
    function.arguments_length = arguments_length;
    function.arguments = arguments;
    function.std_function = 0;
    return function;
}

Function createNullTerminatedFunction () {
    Function function;
    function.name = NULL;
    return function;
}

int getFunctionsLength (const Function* list) {
    int length = 0;
    while (list[length].name != NULL) {
        length++;
    }
    return length;
}

void destroyFunction (Function* function) {
    return;
    for (int i = 0; i < function->arguments_length; i++) {
        free(function->arguments[i].name);
        // function arguments are not freed because they are templates, and do not have allocated memory
    }
    // the function does have ownership of the argument list, so it must be freed
    free(function->arguments);

    // the function does not have ownership of the body or it's name (it's name is most likely unallocated anyways), so it must not be freed
}

#endif

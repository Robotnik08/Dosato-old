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
 * @param return_type The return type of the function
 * @param std Whether or not the function is a standard function
 * @return The function
 */
Function createFunction (char* name, Node* body, Argument* arguments, int arguments_length, DataType return_type, int std);

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



Function createFunction (char* name, Node* body, Argument* arguments, int arguments_length, DataType return_type, int std) {
    Function function;
    function.name = name;
    function.body = body;
    function.arguments_length = arguments_length;
    function.arguments = arguments;
    function.std_function = std;
    function.return_type = return_type;
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
    if (function->arguments != NULL) free(function->arguments);

    // the function does not have ownership of the body or it's name (it's name is most likely unallocated anyways), so it must not be freed
}
#endif

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "ast.h"
#include "strtools.h"
#include "log.h"
#include "process.h"

/**
 * @brief Sets the appropriate error code and prepares the process to throw an error
 * @param process The process to set the error code for
 * @param ast_index The index of the AST node that caused the error
 * @param error_code The error code to set
 * @param location The location of the error (token index)
*/
int error (Process* process, int ast_index, int error_code, int location) {
    process->error_code = error_code;
    process->error_ast_index = ast_index;
    process->error_location = location;
    process->running = 0;
    return error_code;
}

#endif /* ERROR_H */

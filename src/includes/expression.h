#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// general includes
#include "token.h"
#include "node.h"
#include "parser.h"
#include "ast.h"
#include "strtools.h"
#include "log.h"
#include "variable.h"
#include "function.h"
#include "scope.h"
#include "error.h"
#include "log.h"

// specific includes
#include "operator.h"


/**
 * @brief Parse an expression
 * @param process The process to run
 * @param node The node to parse
 * @param return_value The return value of the expression
 * @return The type of the return value (D_NULL means error)
*/
int parseExpression (Variable* var, Process* process, Node* node);

/**
 * @brief Parse a literal
 * @param literal The literal to parse
 * @return The variable
*/
int parseLiteral (Variable* var, Process* process, Node* literal);


int parseExpression (Variable* var, Process* process, Node* node) {
    if (node->text == "-invalid") return error(process, process->error_ast_index, ERROR_INVALID_EXPRESSION, node->start);
    switch (node->type)
    {
        default:
            return error(process, node->start, ERROR_INVALID_EXPRESSION, node->start);
            break;
        case NODE_LITERAL:
            if (parseLiteral(var, process, node)) return error(process, node->start, ERROR_INVALID_EXPRESSION, node->start);
            break;
        case NODE_IDENTIFIER:
            // NOT IMPLEMENTED
            break;
        case NODE_EXPRESSION:
            Variable* left = malloc(sizeof(Variable));
            Variable* right = malloc(sizeof(Variable));
            *left = createNullTerminatedVariable();
            *right = createNullTerminatedVariable();
            if (parseExpression(left, process, &node->body[0])) return error(process, process->error_ast_index, ERROR_INVALID_EXPRESSION, node->body[0].start);
            if (parseExpression(right, process, &node->body[2])) return error(process, process->error_ast_index, ERROR_INVALID_EXPRESSION, node->body[2].start);
            OperatorType operator = process->code->tokens[node->body[1].start].carry;
            
            if (left->type == D_NULL || right->type == D_NULL) {
                return error (process, process->error_ast_index, ERROR_INVALID_EXPRESSION, node->start);
            }
            int res = 0;
            switch (operator) {
                default:
                    return error(process, process->error_ast_index, ERROR_INVALID_OPERATOR, node->start);
                    break;
                case OPERATOR_ADD:
                    res = add(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_SUBTRACT:
                    res = subtract(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_MULTIPLY:
                    res = multiply(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_DIVIDE:
                    res = divide(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_MODULO:
                    res = modulo(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_XOR:
                    res = xor(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_AND:
                    res = and(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;
                case OPERATOR_OR:
                    res = or(var, process, left, right);
                    if (res) return error(process, process->error_ast_index, res, node->start);
                    break;

                
            }
            destroyVariable(left);
            destroyVariable(right);
            free (left);
            free (right);
            break;
        case NODE_UNARY_EXPRESSION:
            // NOT IMPLEMENTED
            break;
    }
    return 0; // success
}
    

int parseLiteral (Variable* var, Process* process, Node* literal) {
    DataType type = D_NULL;
    void* value = NULL;
    if (strsur(literal->text, '\'') || strsur(literal->text, '"')) {
        type = TYPE_STRING;
        char* str = removeLastAndFirstChar(literal->text, 1);

        // parse escape sequences
        strrep(str, "\\n", "\n");
        strrep(str, "\\t", "\t");
        strrep(str, "\\r", "\r");
        strrep(str, "\\0", "\0");
        strrep(str, "\\\\", "\\");
        strrep(str, "\\\"", "\"");
        strrep(str, "\\'", "\'");
        strrep(str, "\\a", "\a");
        strrep(str, "\\b", "\b");
        strrep(str, "\\f", "\f");
        strrep(str, "\\v", "\v");

        value = malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(value, str);
        free(str);
    } else {
        int dot = strchl(literal->text, '.');
        if (dot == 1) {
            if (literal->text[strlen(literal->text)-1] == 'F') {
                type = TYPE_FLOAT;
            } else {
                type = TYPE_DOUBLE;
            }
            value = malloc(sizeof(double));
            *(double*)value = atof(literal->text);
        } else if (!dot) {
            if (literal->text[strlen(literal->text)-1] == 'F') {
                return error(process, process->error_ast_index, ERROR_INVALID_LITERAL, literal->start);
            }
            type = TYPE_ULONG;
            value = malloc(sizeof(unsigned long long));
            *(unsigned long long*)value = atoll(literal->text);
        }
    }
    if (value == NULL || type == D_NULL) {
        return error(process, process->error_ast_index, ERROR_INVALID_LITERAL, literal->start);
    }
    destroyVariable(var); // free the memory of the variable
    *var = createVariable("-lit", type, value, 1, 0); // set the new value
    return 0;
}
#endif

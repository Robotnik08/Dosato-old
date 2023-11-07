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
 * @param var The variable to set the return value to
 * @param process The process to run
 * @param node The node to parse
 * @return The status
 * @note This function is recursive
*/
int parseExpression (Variable* var, Process* process, Node* node);

/**
 * @brief Parse a refrence expression
 * @param process The process to run
 * @param node The node to parse
 * @return The variable (error code is set in the process)
*/
int parseRefrenceExpression (Variable** var, Process* process, Node* node);

/**
 * @brief Parse a literal
 * @param var The variable to set the return value to (make sure to free it, the old value is destroyed)
 * @param process The process to run
 * @param literal The literal to parse
 * @return The variable
*/
int parseLiteral (Variable* var, Process* process, Node* literal);

/**
 * @brief Set the value of a variable
 * @param left The variable to set the value of
 * @param right The variable to set the value to
 * @param op The operator to use
 * @return The status
*/
int setVariableValue (Variable* left, Variable* right, OperatorType op);

/**
 * @brief Parse an array expression
 * @param var The variable to set the return value to
 * @param process The process to run
 * @param node The node to parse
 * @param return_value The return value of the expression
 * @return The status, or when it's a refrence, the pointer to the variable
 * @note This function is recursive
*/
int parseArrayExpression (Variable* var, Process* process, Node* node);


int parseExpression (Variable* var, Process* process, Node* node) {
    if (node->text == "-invalid") return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
    Variable* left;
    Variable* right;
    OperatorType operator;
    int res = 0;
    switch (node->type)
    {
        default:
            return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
            break;
        case NODE_LITERAL:
            int lit = parseLiteral(var, process, node);
            if (lit) return lit;
            break;
        case NODE_IDENTIFIER:
            Variable* ref = getVariable(&process->main_scope, node->text);
            if (ref == NULL) return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_UNDEFINED_VARIABLE, getTokenStart(process, node->start));
            destroyVariable(var);
            *var = cloneVariable(ref);
            break;
        case NODE_EXPRESSION:
            if (getNodeBodyLength(node->body) == 1 && (node->body[0].type == NODE_EXPRESSION || node->body[0].type == NODE_LITERAL || node->body[0].type == NODE_IDENTIFIER || node->body[0].type == NODE_UNARY_EXPRESSION || node->body[0].type == NODE_ARRAY_EXPRESSION)) {
                int res = parseExpression(var, process, &node->body[0]);
                if (res) return res;
                return 0;
            }

            left = malloc(sizeof(Variable));
            right = malloc(sizeof(Variable));
            *left = createNullTerminatedVariable();
            *right = createNullTerminatedVariable();
            int left_parse = parseExpression(left,  process, &node->body[0]);
            if (left_parse) return left_parse;
            int right_parse = parseExpression(right, process, &node->body[2]);
            if (right_parse) return right_parse;
            operator = process->code->tokens[node->body[1].start].carry;

            if (left->type == D_NULL || right->type == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
            }
            switch (operator) {
                default:
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_OPERATOR, getTokenStart(process, node->start));
                    break;

                // arithmetic operators
                case OPERATOR_ADD:
                    res = add(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_SUBTRACT:
                    res = subtract(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_MULTIPLY:
                    res = multiply(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_DIVIDE:
                    res = divide(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_MODULO:
                    res = modulo(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_XOR:
                    res = xor(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_AND:
                    res = and(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_OR:
                    res = or(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;

                // logical operators
                case OPERATOR_OR_OR:
                    res = logic_or(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_AND_AND:
                    res = logic_and(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                
                // comparison operators
                case OPERATOR_EQUAL:
                    res = equal(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_NOT_EQUAL:
                    res = not_equal(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_LESS:
                    res = less_than(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_GREATER:
                    res = greater_than(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_LESS_EQUAL:
                    res = less_than_or_equal(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
                case OPERATOR_GREATER_EQUAL:
                    res = greater_than_or_equal(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;


                case OPERATOR_HASH:
                    res = hash(var, left, right);
                    if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                    break;
            }
            
            if (!strcmp(left->name, "-lit")) {
                destroyVariable(left);
                free (left);
            } else if (!strcmp(left->name, "-litdel")) {
                destroyVariableRef(left);
                free (left);
            }
            
            if (!strcmp(right->name, "-lit")) {
                destroyVariable(right);
                free (right);
            } else if (!strcmp(right->name, "-litdel")) {
                destroyVariableRef(right);
                free (right);
            }
            break;
        case NODE_UNARY_EXPRESSION:
            right = malloc(sizeof(Variable));
            *right = createNullTerminatedVariable();

            if (node->body[0].type == NODE_OPERATOR) {
                if (parseExpression(right, process, &node->body[1])) return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->body[1].start));
                if (right->type == D_NULL) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
                }

                operator = process->code->tokens[node->body[0].start].carry;
                switch (operator) {
                    default:
                        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_OPERATOR, getTokenStart(process, node->start));
                        break;
                    case OPERATOR_NOT:
                        res = not(var, right);
                        if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                        break;
                    case OPERATOR_NOT_BITWISE:
                        res = not_bitwise(var, right);
                        if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                        break;
                    case OPERATOR_SUBTRACT:
                        res = negative(var, right);
                        if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));
                        break;
                }
            }
            else if (node->body[0].type == NODE_OPERATOR_CAST) {
                // this is never a reference, since it is a cast
                if (parseExpression(right, process, &node->body[1])) return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->body[1].start));
                if (right->type == D_NULL) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
                }

                DataType castType = process->code->tokens[node->body[0].start + 1].carry;
                res = castValue(right, castType);
                if (res) return error(process, getLastScope(&process->main_scope)->running_ast, res, getTokenStart(process, node->start));

                destroyVariable(var);
                *var = cloneVariable(right);
            }
            
            if (!strcmp(right->name, "-lit")) {
                destroyVariable(right);
                free (right);
            }
            break;
        
        case NODE_ARRAY_EXPRESSION:
            int res = parseArrayExpression(var, process, node);
            if (res) return res;
            break;
    }
    return 0; // success
}
    
int parseRefrenceExpression (Variable** var, Process* process, Node* node) {
    if (node->text == "-invalid") return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
    Variable* left;
    Variable* right;
    OperatorType operator;
    int res = 0;
    switch (node->type)
    {
        default:
            return error(process, node->start, ERROR_INVALID_REFRENCE_EXPRESSION, getTokenStart(process, node->start));
            break;
        case NODE_IDENTIFIER:
            Variable* ref = getVariable(&process->main_scope, node->text);
            if (ref == NULL) return error(process, node->start, ERROR_UNDEFINED_VARIABLE, getTokenStart(process, node->start));
            *var = ref;
            break;
        case NODE_EXPRESSION:
            if (getNodeBodyLength(node->body) == 1 && node->body[0].type == NODE_EXPRESSION) {
                int res = parseRefrenceExpression(var, process, &node->body[0]);
                if (res) return res;
                return 0;
            }
            right = malloc(sizeof(Variable));
            *right = createNullTerminatedVariable();
            int left_parse = parseRefrenceExpression(&left, process, &node->body[0]);
            if (left_parse) return left_parse;
            int right_parse = parseExpression(right, process, &node->body[2]);
            if (right_parse) return right_parse;
            operator = process->code->tokens[node->body[1].start].carry;

            if (left->type == D_NULL || right->type == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_REFRENCE_EXPRESSION, getTokenStart(process, node->start));
            }
            switch (operator) {
                default:
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_REFRENCE_EXPRESSION, getTokenStart(process, node->start));
                    break;
                case OPERATOR_HASH:
                    int code = hash_refrence(var, left, right);
                    if (code) return error(process, getLastScope(&process->main_scope)->running_ast, code, getTokenStart(process, node->start));
                    break;
            }
            
            if (!strcmp(right->name, "-lit")) {
                destroyVariable(right);
                free (right);
            }
            break;
    }
    return 0; // success
}



int parseLiteral (Variable* var, Process* process, Node* literal) {
    DataType type = D_NULL;
    void* value = NULL;
    if (strsur(literal->text, '"')) {
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
        str = NULL;
    } else if (strsur(literal->text, '\'')) {
        type = TYPE_CHAR;
        char* str = removeLastAndFirstChar(literal->text, 1);
        // parse escape sequences
        strrep(str, "\\n", "\n");
        strrep(str, "\\t", "\t");
        strrep(str, "\\r", "\r");
        strrep(str, "\\\\", "\\");
        strrep(str, "\\\"", "\"");
        strrep(str, "\\'", "\'");
        strrep(str, "\\a", "\a");
        strrep(str, "\\b", "\b");
        strrep(str, "\\f", "\f");
        strrep(str, "\\v", "\v");

        // null terminated character is parsed differently
        if (strlen(str) == 2) {
            if (str[0] == '\\' && str[1] == '0') {
                value = malloc(sizeof(char));
                *(char*)value = '\0';
                free(str);
                str = NULL;
            } else {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_CHAR, getTokenStart(process, literal->start));
            }
        } else {
            if (strlen(str) != 1) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_CHAR, getTokenStart(process, literal->start));
            }
            value = malloc(sizeof(char));
            *(char*)value = str[0];
            free(str);
            str = NULL;
        }
    } else {
        int dot = strchl(literal->text, '.');
        if (dot == 1) {
            char* num = malloc(sizeof(char) * (strlen(literal->text) + 1));
            strcpy(num, literal->text);
            if (literal->text[strlen(literal->text)-1] == 'F') {
                num[strlen(num)-1] = '\0'; // remove the F
                type = TYPE_FLOAT;
                value = malloc(sizeof(float));
                *(float*)value = atof(num);
                free(num);
            } else {
                type = TYPE_DOUBLE;
                value = malloc(sizeof(double));
                *(double*)value = atof(num);    
            }
        } else if (!dot) {
            if (literal->text[strlen(literal->text)-1] == 'F') {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_NUMBER, getTokenStart(process, literal->start));
            }
            type = TYPE_ULONG;
            value = malloc(sizeof(unsigned long long));
            *(unsigned long long*)value = atoll(literal->text);
        }
    }
    if (value == NULL || type == D_NULL) {
        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_LITERAL, getTokenStart(process, literal->start));
    }
    destroyVariable(var); // free the memory of the variable
    *var = createVariable("-lit", type, value, 1, 0); // set the new value
    return 0;
}


int setVariableValue (Variable* left, Variable* right, OperatorType op) {
    if (left->type != right->type) {
        int castRes = castValue(right, left->type);
        if (castRes) return ERROR_TYPE_MISMATCH;
    }

    switch (op) {
        case OPERATOR_ASSIGN:
            int res = assign(left, right);
            if (res) return res;
            break;
        case OPERATOR_ADD_ASSIGN:
            res = assign_add(left, right);
            if (res) return res;
            break;
        case OPERATOR_SUBTRACT_ASSIGN:
            res = assign_subtract(left, right);
            if (res) return res;
            break;
        case OPERATOR_MULTIPLY_ASSIGN:
            res = assign_multiply(left, right);
            if (res) return res;
            break;
        case OPERATOR_DIVIDE_ASSIGN:
            res = assign_divide(left, right);
            if (res) return res;
            break;
        case OPERATOR_MODULO_ASSIGN:
            res = assign_modulo(left, right);
            if (res) return res;
            break;
        case OPERATOR_AND_ASSIGN:
            res = assign_and(left, right);
            if (res) return res;
            break;
        case OPERATOR_OR_ASSIGN:
            res = assign_or(left, right);
            if (res) return res;
            break;
        case OPERATOR_XOR_ASSIGN:
            res = assign_xor(left, right);
            if (res) return res;
            break;
        default:
            return ERROR_INVALID_OPERATOR;
            break;
    }
    return 0;
}

int parseArrayExpression (Variable* var, Process* process, Node* node) {
    int elements_length = getNodeBodyLength(node->body);
    Variable* elements = malloc(sizeof(Variable) * (elements_length + 1));
    DataType arrayType = TYPE_ARRAY; // the array is in a typeless state until the type is determined by casting
    for (int i = 0; i < elements_length; i++) {
        elements[i] = createNullTerminatedVariable();
        int res = parseExpression(&elements[i], process, &node->body[i]);
        if (res) return res;
        char* indexString = malloc(sizeof(char) * 21);
        sprintf(indexString, "#%d", i);
        free(elements[i].name);
        elements[i].name = indexString;
    }
    elements[elements_length] = createNullTerminatedVariable();
    destroyVariable(var);
    *var = createVariable("-lit", arrayType, elements, 1, 1);
    return 0;
}


#endif

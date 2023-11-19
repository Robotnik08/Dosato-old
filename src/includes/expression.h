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
    int oRes = 0;
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
            if (getNodeBodyLength(node->body) == 1 && 
            (node->body[0].type == NODE_EXPRESSION 
            || node->body[0].type == NODE_LITERAL 
            || node->body[0].type == NODE_IDENTIFIER 
            || node->body[0].type == NODE_UNARY_EXPRESSION 
            || node->body[0].type == NODE_ARRAY_EXPRESSION 
            || node->body[0].type == NODE_BLOCK_EXPRESSION 
            || node->body[0].type == NODE_FUNCTION_IDENTIFIER)) {
                int oRes = parseExpression(var, process, &node->body[0]);
                if (oRes) return oRes;
                return 0;
            }

            left = malloc(sizeof(Variable));
            right = malloc(sizeof(Variable));
            *left = createNullTerminatedVariable();
            *right = createNullTerminatedVariable();
            int left_parse = parseExpression(left,  process, &node->body[0]);
            if (left_parse) return left_parse;
            if (left->type.dataType == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->body[0].start));
            }
            int right_parse = parseExpression(right, process, &node->body[2]);
            if (right_parse) return right_parse;
            if (right->type.dataType == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->body[2].start));
            }
            operator = process->code->tokens[node->body[1].start].carry;

            if (left->type.dataType == D_NULL || right->type.dataType == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
            }
            switch (operator) {
                default:
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_OPERATOR, getTokenStart(process, node->start));
                    break;

                // arithmetic operators
                case OPERATOR_ADD:
                    oRes = add(var, left, right);
                    break;
                case OPERATOR_SUBTRACT:
                    oRes = subtract(var, left, right);
                    break;
                case OPERATOR_MULTIPLY:
                    oRes = multiply(var, left, right);
                    break;
                case OPERATOR_POWER:
                    oRes = pow_op(var, left, right);
                    break;
                case OPERATOR_DIVIDE:
                    oRes = divide(var, left, right);
                    break;
                case OPERATOR_ROOT:
                    oRes = root(var, left, right);
                    break;
                case OPERATOR_MODULO:
                    oRes = modulo(var, left, right);
                    break;
                case OPERATOR_XOR:
                    oRes = xor(var, left, right);
                    break;
                case OPERATOR_AND:
                    oRes = and(var, left, right);
                    break;
                case OPERATOR_OR:
                    oRes = or(var, left, right);
                    break;
                case OPERATOR_SHIFT_LEFT:
                    oRes = bitshift_left(var, left, right);
                    break;
                case OPERATOR_SHIFT_RIGHT:
                    oRes = bitshift_right(var, left, right);
                    break;

                // logical operators
                case OPERATOR_OR_OR:
                    oRes = logic_or(var, left, right);
                    break;
                case OPERATOR_AND_AND:
                    oRes = logic_and(var, left, right);
                    break;
                
                // comparison operators
                case OPERATOR_EQUAL:
                    oRes = equal(var, left, right);
                    break;
                case OPERATOR_NOT_EQUAL:
                    oRes = not_equal(var, left, right);
                    break;
                case OPERATOR_LESS:
                    oRes = less_than(var, left, right);
                    break;
                case OPERATOR_GREATER:
                    oRes = greater_than(var, left, right);
                    break;
                case OPERATOR_LESS_EQUAL:
                    oRes = less_than_or_equal(var, left, right);
                    break;
                case OPERATOR_GREATER_EQUAL:
                    oRes = greater_than_or_equal(var, left, right);
                    break;

                // special operators
                case OPERATOR_HASH:
                    oRes = hash(var, left, right);
                    break;
                case OPERATOR_MAX:
                    oRes = max_op(var, left, right);
                    break;
                case OPERATOR_MIN:
                    oRes = min_op(var, left, right);
                    break;
            }
            if (oRes) return error(process, getLastScope(&process->main_scope)->running_ast, oRes, getTokenStart(process, node->start));
            
            if (!strcmp(left->name, "-lit")) {
                destroyVariable(left);
                free (left);
            }
            
            if (!strcmp(right->name, "-lit")) {
                destroyVariable(right);
                free (right);
            }
            break;
        case NODE_UNARY_EXPRESSION:
            right = malloc(sizeof(Variable));
            *right = createNullTerminatedVariable();

            if (node->body[0].type == NODE_OPERATOR) {
                int rRes = parseExpression(right, process, &node->body[1]);
                if (rRes) return rRes;
                if (right->type.dataType == D_NULL) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
                }

                operator = process->code->tokens[node->body[0].start].carry;
                switch (operator) {
                    default:
                        return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_OPERATOR_NOT_UNARY, getTokenStart(process, node->start));
                        break;
                    case OPERATOR_NOT:
                        oRes = not(var, right);
                        break;
                    case OPERATOR_NOT_BITWISE:
                        oRes = not_bitwise(var, right);
                        break;
                    case OPERATOR_SUBTRACT:
                        oRes = negative(var, right);
                        break;
                    case OPERATOR_ROOT:
                        oRes = sqroot(var, right);
                        break;
                    case OPERATOR_ABSOLUTE:
                        oRes = absolute(var, right);
                        break;
                }
                if (oRes) return error(process, getLastScope(&process->main_scope)->running_ast, oRes, getTokenStart(process, node->start));
            }
            else if (node->body[0].type == NODE_OPERATOR_CAST) {
                // this is never a reference, since it is a cast
                int rRes = parseExpression(right, process, &node->body[1]);
                if (rRes) return rRes;
                if (right->type.dataType == D_NULL) {
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
                }

                Type castType = (Type){.dataType = D_NULL, .array = 0};
                int tRes = getTypeFromCastNode(process, &castType, &node->body[0]);
                if (tRes) return error(process, getLastScope(&process->main_scope)->running_ast, tRes, getTokenStart(process, node->start));
                oRes = castValue(right, castType);
                if (oRes) return error(process, getLastScope(&process->main_scope)->running_ast, oRes, getTokenStart(process, node->start));
                destroyVariable(var);
                *var = cloneVariable(right);
            }
            
            if (!strcmp(right->name, "-lit")) {
                destroyVariable(right);
                free (right);
            }
            break;
        
        case NODE_ARRAY_EXPRESSION:
            oRes = parseArrayExpression(var, process, node);
            if (oRes) return oRes;
            break;

        case NODE_FUNCTION_IDENTIFIER:
        case NODE_BLOCK_EXPRESSION:
            oRes = parseCall(process, node);
            // change var to the return value
            destroyVariable(var);
            *var = cloneVariable(getReturnValue(process));
            free (var->name);
            var->name = malloc(sizeof(char) * 5);
            strcpy(var->name, "-lit");
            if (oRes) return oRes;
            break;
    }
    return 0; // success
}
    
int parseRefrenceExpression (Variable** var, Process* process, Node* node) {
    if (var == NULL) return 0;
    if (node->text == "-invalid") return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_EXPRESSION, getTokenStart(process, node->start));
    Variable* left;
    Variable* right;
    OperatorType operator;
    int oRes = 0;
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
                oRes = parseRefrenceExpression(var, process, &node->body[0]);
                if (oRes) return oRes;
                return 0;
            }
            right = malloc(sizeof(Variable));
            *right = createNullTerminatedVariable();
            int left_parse = parseRefrenceExpression(&left, process, &node->body[0]);
            if (left_parse) return left_parse;
            int right_parse = parseExpression(right, process, &node->body[2]);
            if (right_parse) return right_parse;
            operator = process->code->tokens[node->body[1].start].carry;

            if (left->type.dataType == D_NULL || right->type.dataType == D_NULL) {
                return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_REFRENCE_EXPRESSION, getTokenStart(process, node->start));
            }
            switch (operator) {
                default:
                    return error(process, getLastScope(&process->main_scope)->running_ast, ERROR_INVALID_REFRENCE_EXPRESSION, getTokenStart(process, node->start));
                    break;
                case OPERATOR_HASH:
                    oRes = hash_refrence(var, left, right);
                    if (oRes) return error(process, getLastScope(&process->main_scope)->running_ast, oRes, getTokenStart(process, node->start));
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
    *var = createVariable("-lit", type, value, 0, 0); // set the new value
    return 0;
}


int setVariableValue (Variable* left, Variable* right, OperatorType op) {

    // setting an array
    if (left->type.array) {
        if (op == OPERATOR_ASSIGN) {
            if (!compareType(left->type, right->type)) {
                int castRes = castValue(right, left->type);
                if (castRes) return ERROR_TYPE_MISMATCH;
            }
            // destroy the old array and it's contents
            int len = getVariablesLength(left->value);
            for (int i = 0; i < len; i++) {
                destroyVariable(&((Variable*)left->value)[i]);
            }
            free (left->value); // free the array

            // copy the new array
            left->value = malloc(sizeof(Variable) * (getVariablesLength(right->value) + 1));
            for (int i = 0; i < getVariablesLength(right->value); i++) {
                ((Variable*)left->value)[i] = cloneVariable(&((Variable*)right->value)[i]);
            }
            ((Variable*)left->value)[getVariablesLength(right->value)] = createNullTerminatedVariable();
            return 0; 
        } else {
            int len = getVariablesLength(left->value);
            Variable* newArray = NULL;
            if (op == OPERATOR_ADD_ASSIGN) {
                if (!compareType(left->type, right->type)) {
                    int castRes = castValue(right, left->type);
                    if (castRes) return castRes;
                }
                // allocate new memory for the array
                newArray = malloc(sizeof(Variable) * (len + getVariablesLength(right->value) + 1));
                for (int i = 0; i < len; i++) {
                    newArray[i] = cloneVariable(&((Variable*)left->value)[i]);
                }
                for (int i = 0; i < getVariablesLength(right->value); i++) {
                    newArray[len + i] = cloneVariable(&((Variable*)right->value)[i]);
                }
                newArray[len + getVariablesLength(right->value)] = createNullTerminatedVariable();
                
            } else if (op == OPERATOR_SUBTRACT_ASSIGN) {
                if (!compareType((Type){.dataType = TYPE_LONG, .array = 0}, right->type)) {
                    int castRes = castValue(right, (Type){.dataType = TYPE_LONG, .array = 0});
                    if (castRes) return castRes;
                }

                // check if the array is long enough
                if (*(long*)right->value > len || *(long*)right->value < 0) {
                    return ERROR_ARRAY_OUT_OF_BOUNDS;
                }

                // allocate new memory for the array
                newArray = malloc(sizeof(Variable) * (len - *(long*)right->value + 1));
                for (int i = 0; i < len - *(long*)right->value; i++) {
                    newArray[i] = cloneVariable(&((Variable*)left->value)[i]);
                }
                newArray[len - *(long*)right->value] = createNullTerminatedVariable();
            }
            // free the old values
            for (int i = 0; i < len; i++) {
                destroyVariable(&((Variable*)left->value)[i]);
            }
            free(left->value);
            left->value = newArray;
            return 0;
        }
        return ERROR_INVALID_OPERATOR;  
    }

    // setting a regular variable
    if (!compareType(left->type, right->type)) {
        int castRes = castValue(right, left->type);
        if (castRes) return ERROR_TYPE_MISMATCH;
    }
    switch (op) {
        case OPERATOR_ASSIGN:
            int oRes = assign(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_ADD_ASSIGN:
            oRes = assign_add(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_SUBTRACT_ASSIGN:
            oRes = assign_subtract(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_MULTIPLY_ASSIGN:
            oRes = assign_multiply(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_DIVIDE_ASSIGN:
            oRes = assign_divide(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_MODULO_ASSIGN:
            oRes = assign_modulo(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_AND_ASSIGN:
            oRes = assign_and(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_OR_ASSIGN:
            oRes = assign_or(left, right);
            if (oRes) return oRes;
            break;
        case OPERATOR_XOR_ASSIGN:
            oRes = assign_xor(left, right);
            if (oRes) return oRes;
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
    Type arrayType = (Type){TYPE_ARRAY, 0}; // the array is in a typeless state until the type is determined by casting

    for (int i = 0; i < elements_length; i++) {
        elements[i] = createNullTerminatedVariable();
        int oRes = parseExpression(&elements[i], process, &node->body[i]);
        if (oRes) return oRes;
        char* indexString = malloc(sizeof(char) * 21);
        sprintf(indexString, "%d", i);
        free(elements[i].name);
        elements[i].name = indexString;
        elements[i].constant = 0;
        if (i == 0) {
            arrayType = elements[i].type;
        } else if (!compareType(arrayType, elements[i].type)) {
            int cRes = castValue(&elements[i], arrayType);
            if (cRes) {
                return error(process, getLastScope(&process->main_scope)->running_ast, cRes, getTokenStart(process, node->body[i].start));
            }
        }
    }
    elements[elements_length] = createNullTerminatedVariable();
    destroyVariable(var);
    *var = createVariable("-lit", arrayType.dataType, elements, 0, arrayType.array + 1);
    return 0;
}


#endif

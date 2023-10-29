/**
 * @author Sebastiaan Heins
 * @file ast_debug.h
 * @brief Helpful functions for debugging the AST, these functions are not used in the interpreter but they are very helpful for debugging
 * @version 1.0
 * @date 10-10-2023
*/

#ifndef AST_DEBUG_H
#define AST_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"



/**
 * @brief Get the string version of a node type
 * @param type The node type
 * @return The string version of the node type
*/
char* getNodeTypeString(NodeType type);

/**
 * @brief Get the string of the full code from a node, start of first token and end of last token
 * @param full_code The full code
 * @param tokens The list of tokens
 * @param node The node to get the string version of
 * @return The string version of the node (don't forget to free it)
*/
char* getStringFromNode (const char* full_code, const Token* tokens, const Node* node);

/**
 * @brief Print a node
 * @param node The node to print
 * @param depth The depth of the node (this is used for indentation, root should be 1)
 * @param isRoot Whether or not the node is the root node (boolean)
*/
void printNode (const char* full_code, const Token* tokens, const Node* node, const int depth, int isRoot);

char* getNodeTypeString(NodeType type) {

    switch (type) {
        case NODE_NULL:
            return "NODE_NULL";
            
        case NODE_PROGRAM:
            return "program";
            
        case NODE_FUNCTION_CALL:
            return "function call";
            
        case NODE_MAKE_VAR:
            return "make_var";
            
        case NODE_SET_VAR:
            return "set_var";
            
        case NODE_FUNCTION_DECLARATION:
            return "function declaration";
            
        case NODE_FUNCTION_DECLARATION_ARGUMENTS:
            return "function_declaration_arguments";
            
        case NODE_FUNCTION_DECLARATION_ARGUMENT:
            return "function_declaration_argument";
            
        case NODE_EXPRESSION:
            return "expression";
            
        case NODE_UNARY_EXPRESSION:
            return "unary_expression";
            
        case NODE_STATEMENT:
            return "statement";
            
        case NODE_LITERAL:
            return "literal";
            
        case NODE_IDENTIFIER:   
            return "identifier";
            
        case NODE_OPERATOR: 
            return "operator";
            
        case NODE_BLOCK:
            return "block";
            
        case NODE_FUNCTION_IDENTIFIER:
            return "function_identifier";
            
        case NODE_ARGUMENTS:    
            return "arguments";
            
        case NODE_TYPE_IDENTIFIER:
            return "type_identifier";
            
        case NODE_ARRAY_DECLARATION:
            return "array_declaration";
            
        case NODE_ARRAY_EXPRESSION:
            return "array_expression";
            
        case NODE_WHEN:
            return "when";
            
        case NODE_WHILE:
            return "while";
            
        case NODE_ELSE:
            return "else";
            
        case NODE_CATCH:    
            return "catch";
            
        case NODE_INTO:
            return "into";
            
        case NODE_THEN:
            return "then";
            
        case NODE_END:
            return "NODE_END";
            
        default:
            return "Unknown"; // Return a default value for invalid types
            
    }
}
 
char* getStringFromNode (const char* full_code, const Token* tokens, const Node* node) {
    char* string = NULL;
    if (node->start > node->end) {
        return "-invalid";
    }
    int string_length = tokens[node->end].end - tokens[node->start].start + 2;
    string = malloc(sizeof(char) * string_length);

    int stringIndex = 0;
    for (int i = tokens[node->start].start; i <= tokens[node->end].end; i++) {
        string[stringIndex++] = full_code[i] == '\n' ? ' ' : full_code[i];
    }
    string[stringIndex] = '\0';
    return string;
}

void printNode (const char* full_code, const Token* tokens, const Node* node, int depth, int isRoot) {
    if (isRoot) {
        printf("{\n");
        printf("  \"root\": ");
    } else {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
    }
    printf("{\n");

    // Print the node position
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"start, end\": [%i, %i],\n", node->start, node->end);

    // Print the node type
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"type\": \"%s\",\n", getNodeTypeString(node->type));

    
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"text\": \"%s\"", node->text);

    if (node->body != NULL) {
        printf(",\n");
        for (int i = 0; i < depth +1; i++) {
            printf("  ");
        }
        printf("\"body\": [\n");
        for (int i = 0; i < getNodeBodyLength(node->body); i++) {
            printNode(full_code, tokens, &node->body[i], depth + 2, 0);
            if (i != getNodeBodyLength(node->body) - 1) {
                printf(",\n");
            }
        }
        printf("\n");
        for (int i = 0; i < depth+1; i++) {
            printf("  ");
        }
        printf("]\n");
    } else {
        printf("\n");
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("}");
    if (isRoot) {
        printf("\n}");
    }
}

#endif
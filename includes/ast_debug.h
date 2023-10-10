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
 * @param string The string version of the node
 * @return The string version of the node (don't forget to free it)
*/
void getStringFromNode (const char* full_code, const Token* tokens, const Node* node, char** string);

/**
 * @brief Print a node
 * @param node The node to print
 * @param depth The depth of the node (this is used for indentation, root should be 1)
 * @param isRoot Whether or not the node is the root node (boolean)
*/
void printNode (const char* full_code, const Token* tokens, const Node* node, const int depth, int isRoot);

char* getNodeTypeString(NodeType type) {
    char* nodeTypeString = NULL;

    switch (type) {
        case NODE_NULL:
            nodeTypeString = "NODE_NULL";
            break;
        case NODE_PROGRAM:
            nodeTypeString = "program";
            break;
        case NODE_FUNCTION_CALL:
            nodeTypeString = "function call";
            break;
        case NODE_MAKE_VAR:
            nodeTypeString = "make_var";
            break;
        case NODE_SET_VAR:
            nodeTypeString = "set_var";
            break;
        case NODE_FUNCTION_DECLARATION:
            nodeTypeString = "function declaration";
            break;
        case NODE_EXPRESSION:
            nodeTypeString = "expression";
            break;
        case NODE_UNARY_EXPRESSION:
            nodeTypeString = "unary_expression";
            break;
        case NODE_STATEMENT:
            nodeTypeString = "statement";
            break;
        case NODE_LITERAL:
            nodeTypeString = "literal";
            break;
        case NODE_IDENTIFIER:   
            nodeTypeString = "identifier";
            break;
        case NODE_OPERATOR: 
            nodeTypeString = "operator";
            break;
        case NODE_BLOCK:
            nodeTypeString = "block";
            break;
        case NODE_FUNCTION_IDENTIFIER:
            nodeTypeString = "function identifier";
            break;
        case NODE_ARGUMENTS:    
            nodeTypeString = "arguments";
            break;
        case NODE_TYPE_IDENTIFIER:
            nodeTypeString = "type_identifier";
            break;
        case NODE_WHEN:
            nodeTypeString = "when";
            break;
        case NODE_WHILE:
            nodeTypeString = "while";
            break;
        case NODE_ELSE:
            nodeTypeString = "else";
            break;
        case NODE_CATCH:    
            nodeTypeString = "catch";
            break;
        case NODE_INTO:
            nodeTypeString = "into";
            break;
        case NODE_THEN:
            nodeTypeString = "then";
            break;
        case NODE_END:
            nodeTypeString = "NODE_END";
            break;
        default:
            nodeTypeString = "Unknown"; // Return a default value for invalid types
            break;
    }

    return nodeTypeString;
}
 
void getStringFromNode (const char* full_code, const Token* tokens, const Node* node, char** string) {
    if (node->start > node->end) {
        *string = "Error: start > end";
        return;
    }
    *string = (char*)malloc(sizeof(char) * (tokens[node->end].end - tokens[node->start].start + 1));

    int stringIndex = 0;
    for (int i = tokens[node->start].start; i <= tokens[node->end].end; i++) {
        (*string)[stringIndex++] = full_code[i] == '\n' ? ' ' : full_code[i];
    }
    (*string)[stringIndex] = '\0';
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
    printf("\"start\": %i,\n", node->start);

    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"end\": %i,\n", node->end);

    // Print the node type
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"type\": \"%s\",\n", getNodeTypeString(node->type));

    
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    char *stringFromNode;
    getStringFromNode(full_code, tokens, node, &stringFromNode);
    printf("\"text\": \"%s\"", stringFromNode);
    free(stringFromNode);

    if (node->body != NULL) {
        printf(",\n");
        for (int i = 0; i < depth +1; i++) {
            printf("  ");
        }
        printf("\"body\": [\n");
        for (int i = 0; i < getBodyLength(node->body); i++) {
            printNode(full_code, tokens, &node->body[i], depth + 2, 0);
            if (i != getBodyLength(node->body) - 1) {
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
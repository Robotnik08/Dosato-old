#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"

typedef struct Node Node;

struct Node {
    int start, end;
    NodeType type;
    Node* body;
};

/**
 * @brief Create a null terminated node
 * @return The null terminated node
*/
Node createNullTerminatedNode();

/**
 * @brief Create a null terminated node
 * @return The null terminated node
*/
void addToBody (Node** body, Node node);

/**
 * @brief Get the full line starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The full line
*/
int getFullLine (Token* tokens, const int start);

/**
 * @brief Get the block starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The full block
*/
int getBlock (Token* tokens, const int start);

/**
 * @brief Parse a list of tokens into an AST
 * @param tokens The list of tokens to parse
 * @param length The length of the list of tokens
 * @return The root node of the AST
*/
Node parse (const char* code, Token* tokens, const int start, const int end, const NodeType type);

/**
 * @brief Print a node
 * @param node The node to print
 * @param depth The depth of the node
*/
void printNode (const Node* node, const int depth, int isRoot);

/**
 * @brief Get the length of a node's body
 * @param nodes The node's body
 * @return The length of the node's body
*/
int getBodyLength (const Node* nodes);



Node createNullTerminatedNode () {
    Node node;
    node.start = 0;
    node.end = 0;
    node.type = NODE_END;
    node.body = NULL;
    return node;
}

void addToBody (Node** body, const Node node) {
    int length = getBodyLength(*body);
    *body = realloc(*body, sizeof(Node) * (length + 2));
    (*body)[length] = node;

    (*body)[length + 1] = createNullTerminatedNode();
}

int getFullLine (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    for (int i = start; i < length; i++) {
        if (tokens[i].type == TOKEN_PARENTHESIS) {
            i = getBlock(tokens, i);
        }
        if (tokens[i].type == TOKEN_SEPARATOR) {
            return i;
        }
    }
    return length-1;
}

int getBlock (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    int targetCarry = tokens[start].carry;
    for (int i = start+1; i < length; i++) {
        if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry == targetCarry) {
            return i;
        }
    }
    return length-1;
}

Node parse (const char* full_code, Token* tokens, const int start, const int end, const NodeType type) {
    // test nodes to be printed
    
    Node root;
    root.start = start;
    root.end = end;
    root.type = type;
    root.body = NULL;
    
    for (int i = start; i < end; i++) {
        if (tokens[i].type == TOKEN_MASTER_KEYWORD) {
            int full_line = getFullLine(tokens, i);
            switch (tokens[i].carry)
            {
                case MASTER_DO:
                    addToBody(&root.body, parse(full_code, tokens, i+1, full_line, NODE_FUNCTION_CALL));
                    i = full_line;
                    break;
                case MASTER_MAKE:
                    // addToBody(&root.body, i, i, NODE_MAKE_VAR);
                    break;
                case MASTER_SET:
                    // addToBody(&root.body, i, i, NODE_SET_VAR);
                    break;
                default:
            }
        }
    }

    return root;
}

void printNode (const Node* node, int depth, int isRoot) {
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
    printf("\"type\": %d", node->type);

    if (node->body != NULL) {
        printf(",\n");
        for (int i = 0; i < depth +1; i++) {
            printf("  ");
        }
        printf("\"body\": [\n");
        for (int i = 0; i < getBodyLength(node->body); i++) {
            printNode(&node->body[i], depth + 2, 0);
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

void destroyNode (Node* node) {
    if (node->body != NULL) {
        for (int i = 0; i < getBodyLength(node->body); i++) {
            destroyNode(&node->body[i]);
        }
        free(node->body);
    }
}

int getBodyLength (const Node* nodes) {
    if (nodes == NULL) return 0;
    int i = 0;
    while (nodes[i].type != NODE_END) {
        i++;
    }
    return i;
}


#endif
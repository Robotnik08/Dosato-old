#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

/**
 * @brief The Node Type is used to represent a node in the AST
*/
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
 * @brief Add a node to the body of a node
 * @param body The body to add the node to
 * @param node The node to add to the body
*/
void addToBody (Node** body, Node node);

/**
 * @brief Get the full line starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of the last token of the line
*/
int getFullLine (Token* tokens, const int start);

/**
 * @brief Get the block starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of thelast token of the block
*/
int getBlock (Token* tokens, const int start);

/**
 * @brief Get the block starting at a token, but reads backwards
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of the first token of the block
*/
int getBlockReverse (Token* tokens, const int start);

/**
 * @brief Get the amount of nodes in a node's body 
 * @param nodes The node's body
 * @return The amount of nodes in the body (excluding the null terminator)
*/
int getBodyLength (const Node* nodes);

/**
 * @brief Destroy a node and all of it's children, freeing the memory
 * @param node The node to destroy
*/
void destroyNode (Node* node);


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
    if (tokens[start].type != TOKEN_PARENTHESIS) return start;
    int length = getTokenAmount(tokens);
    int targetCarry = tokens[start].carry;
    for (int i = start+1; i < length; i++) {
        if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry == targetCarry) {
            return i;
        }
    }
    return start;
}
int getBlockReverse (Token* tokens, const int start) {
    if (tokens[start].type != TOKEN_PARENTHESIS) return start;
    int length = getTokenAmount(tokens);
    int targetCarry = tokens[start].carry;
    for (int i = start-1; i >= 0; i--) {
        if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry == targetCarry) {
            return i;
        }
    }
    return start;
}

int getExpression (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    for (int i = start+1; i < length; i++) {
        if (tokens[i].type == TOKEN_SEPARATOR || (tokens[i].type == TOKEN_PARENTHESIS && !(tokens[i].carry & BRACKET_ROUND)) || tokens[i].type == TOKEN_MASTER_KEYWORD || tokens[i].type == TOKEN_EXT || tokens[i].type == TOKEN_NULL) {
            return i-1;
        }
    }
    return -1;
}

int getSetExpression (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    for (int i = start+1; i < length; i++) {
        if (tokens[i].type == TOKEN_SEPARATOR || (tokens[i].type == TOKEN_PARENTHESIS && !(tokens[i].carry & BRACKET_ROUND)) || tokens[i].type == TOKEN_MASTER_KEYWORD || tokens[i].type == TOKEN_EXT || tokens[i].type == TOKEN_NULL || (tokens[i].type == TOKEN_OPERATOR && tokens[i].carry == OPERATOR_COMMA) || (tokens[i].type == TOKEN_OPERATOR && isAssignmentOperator(tokens[i].carry))) {
            return i-1;
        }
    }
    return -1;
}

int getBodyLength (const Node* nodes) {
    if (nodes == NULL) return 0;
    int i = 0;
    while (nodes[i].type != NODE_END) {
        i++;
    }
    return i;
}

void destroyNode (Node* node) {
    if (node->body != NULL) {
        for (int i = 0; i < getBodyLength(node->body); i++) {
            destroyNode(&node->body[i]);
        }
        free(node->body);
    }
}
#endif
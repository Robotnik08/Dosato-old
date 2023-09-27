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
 * @brief Parse a list of tokens into an AST
 * @param tokens The list of tokens to parse
 * @param length The length of the list of tokens
*/
Node parse (const Token* tokens, const int length);
/**
 * @brief Print a node
 * @param node The node to print
 * @param depth The depth of the node
*/
void printNode (const Node* node, const int depth, int isRoot); 
/**
 * @brief Get the length of a node's body
 * @param nodes The node's body
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


Node parse (const Token* tokens, const int length) {
    // test nodes to be printed
    Node program;
    program.start = 0;
    program.end = length;
    program.type = NODE_PROGRAM;
    program.body = malloc(sizeof(Node)*4);
        program.body[0].start = 0;
        program.body[0].end = 1;
        program.body[0].type = NODE_FUNCTION_DECLARATION;
        program.body[0].body = NULL;

        program.body[1].start = 1;
        program.body[1].end = 2;
        program.body[1].type = NODE_BLOCK;
        program.body[1].body = malloc(sizeof(Node)*3);
            program.body[1].body[0].start = 1;
            program.body[1].body[0].end = 2;
            program.body[1].body[0].type = NODE_FUNCTION_CALL;
            program.body[1].body[0].body = NULL;

            program.body[1].body[1].start = 2;
            program.body[1].body[1].end = 3;
            program.body[1].body[1].type = NODE_FUNCTION_CALL;
            program.body[1].body[1].body = NULL;
            
            program.body[1].body[2] = createNullTerminatedNode();

        program.body[2].start = 3;
        program.body[2].end = 4;
        program.body[2].type = NODE_FUNCTION_CALL;
        program.body[2].body = NULL;

        program.body[3] = createNullTerminatedNode();


    return program;
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


int getBodyLength (const Node* nodes) {
    if (nodes == NULL) return 0;
    int i = 0;
    while (nodes[i].type != NODE_END) {
        i++;
    }
    return i;
}


#endif
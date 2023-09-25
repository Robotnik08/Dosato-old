#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"

// AST node structure
typedef struct AstNode {
    NodeType type;
    int tokenIndex;
    struct AstNode* left;
    struct AstNode* right;
} AstNode;

#endif
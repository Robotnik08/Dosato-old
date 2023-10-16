#ifndef AST_H
#define AST_H

#include "token.h"
#include "node.h"

typedef struct {
    char* full_code;
    Token* tokens;
    Node root;
    char* filename;
} AST;

#endif
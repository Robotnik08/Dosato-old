#ifndef AST_H
#define AST_H

#include "token.h"
#include "node.h"
#include "parser.h"
#include "lexer.h"

typedef struct {
    char* full_code;
    Token* tokens;
    Node root;
    char* filename;
} AST;

/**
 * @brief Load an AST from a file
 * @param filename The name of the file
 * @param full_code The full code of the file
 * @return The AST
 * @warning The AST must be destroyed after use
*/
AST loadAST (const char* filename, const char* full_code);

/**
 * @brief Destroy an AST, freeing all memory
 * @param ast The AST to destroy
*/
void destroyAST (AST* ast);


AST loadAST (const char* filename, const char* full_code) {
    AST ast;
    ast.filename = malloc(strlen(filename));
    strcpy(ast.filename, filename);
    ast.full_code = malloc(strlen(full_code));
    strcpy(ast.full_code, full_code);

    ast.tokens = NULL;
    tokenise(&ast.tokens, ast.full_code, strlen(ast.full_code));

    ast.root = parse(ast.full_code, ast.tokens, 0, getTokenAmount(ast.tokens)-1, NODE_PROGRAM);

    return ast;
}

void destroyAST (AST* ast) {
    destroyNode(&ast->root);
    free(ast->tokens);
    free(ast->full_code);
    free(ast->filename);
}
#endif
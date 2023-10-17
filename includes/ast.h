/**
 * @author Sebastiaan Heins
 * @file ast.h
 * @brief Contains the ast struct and helper functions
 * @version 0.0.4
 * @date 17-10-2023
*/

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
 * @brief Create a null terminated AST, null terminated ASTs are used to represent bounds of AST pointers
 * @return The null terminated AST
*/
AST createNullTerminatedAST ();

/**
 * @brief Check if an AST is null terminated
 * @param ast The AST to check
 * @return Whether or not the AST is null terminated
*/
int checkIfNullTerminatedAST (const AST* ast);

/**
 * @brief Get the length of an array of ASTs
 * @param asts The array of ASTs
 * @return The length of the array
*/
int getASTsLength (const AST* asts);
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


AST createNullTerminatedAST () {
    AST ast;
    ast.filename = NULL;
    return ast;
}

int checkIfNullTerminatedAST (const AST* ast) {
    return ast->filename == NULL;
}

int getASTsLength (const AST* asts) {
    int i = 0;
    while (!checkIfNullTerminatedAST(&asts[i])) {
        i++;
    }
    return i;
}

void addAST (AST** asts, AST ast) {
    int length = getASTsLength(*asts);
    *asts = realloc(*asts, sizeof(AST) * (length + 2));
    (*asts)[length] = ast;
    (*asts)[length+1] = createNullTerminatedAST();
}
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
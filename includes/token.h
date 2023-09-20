#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NULL,
    TOKEN_COMMENT,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_MASTER_KEYWORD,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_SEPARATOR
} TokenType;

typedef struct {
    int start, end;
    TokenType type;
    int carry;
} Token;

#endif
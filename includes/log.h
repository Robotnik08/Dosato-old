#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "str_tools.h"

typedef enum {
    ERROR,
    WARNING,
    GENERIC
} LogType;

typedef enum {
    ERROR_NULL,
    ERROR_SYNTAX,
    ERROR_TYPE,
    ERROR_UNDEFINED,
    ERROR_RECURSION,
    ERROR_MEMORY,
    ERROR_FILE,
    ERROR_RUNTIME,
    ERROR_PARSER,
    ERROR_EXPECTED_MASTER,
    ERROR_EXPECTED_IDENTIFIER,
    ERROR_WRONG_BRACKET_ROUND,
    ERROR_WRONG_BRACKET_SQUARE,
    ERROR_WRONG_BRACKET_CURLY,
    ERROR_EXPECTED_ARGUMENTS,
    ERROR_EXPECTED_ARGUMENT,
    ERROR_EXPECTED_TYPE,
    ERROR_EXPECTED_ASSIGN_OPERATOR,
    ERROR_UNKNOWN

} ErrorType;

const char* ERROR_MESSAGES[] = {
    "No error Occured? (CODE NULL)", 
    "Syntax Error", 
    "Type Mismatch Error", 
    "Undefined Variable Error", 
    "Recursion Limit reached Error", 
    "Memory Error", 
    "File Error", 
    "Runtime Error", 
    "Parser Error",
    "Expected Master Keyword as first token (DO, SET, MAKE)",
    "Expected Identifier",
    "Bracket is incorrect, expected a round bracket ( )",
    "Bracket is incorrect, expected a square bracket [ ]",
    "Bracket is incorrect, expected a curly bracket { }",
    "Expected () after function call",
    "Expected an argument",
    "Expected a type (INT, FLOAT, STRING, BOOL etc)",
    "Expected an assignment operator",
    "Unknown Error"
};

void logText (const LogType type, const char* contents);
void printError (const char* full_code, const int pos, const ErrorType type);

void logText (const LogType type, const char* contents) {
    switch (type) {
        case ERROR:
            printf("ERROR:\n");
            break;
        case WARNING:
            printf("WARNING:\n");
            break;
    }
    printf("%s", contents);
    if (type == ERROR) {
        exit(1);
    }
}
void printError (const char* full_code, const int pos, const ErrorType type) {
    printf("ERROR:\n");
    printf("%s\n", ERROR_MESSAGES[type]);
    printf("At line %i:%i\n", getLine(full_code, pos), getLineCol(full_code, pos));
    exit(type);
}

#endif
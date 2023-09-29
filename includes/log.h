#ifndef LOG_H
#define LOG_H

#include <stdio.h>

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
    "Unknown Error"
};

void logText (const LogType type, const char* contents);
void printError (const int line, const ErrorType type);

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
void printError (const int line, const ErrorType type) {
    printf("ERROR:\n");
    printf("%s\n", ERROR_MESSAGES[type]);
    printf("At line %i\n", line);
    exit(type);
}

#endif
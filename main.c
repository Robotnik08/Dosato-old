#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/str_tools.h"
#include "includes/lang.h"
#include "includes/lexer.h"
#include "includes/parser.h"

// forward declarations
int QUIT (int code);

// global variables
int debug = 0;


int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Use -h or -help for help\n");
        return QUIT(0);
    }
    
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("Commands:\n");
        printf("\t-h, --help: Show this help message\n");
        printf("\t-v, --version: Show the version number\n");
        printf("\t(PROGRAM_NAME): Run a file\n");
        printf("\t(PROGRAM_NAME) -d, --debug: Run a file in debug mode\n");
        
        return QUIT(0);
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("Dosato Version: 0.0.2\n");
        printf("2023, made by Sebastiaan Heins\n");

        return QUIT(0);
    }

    FILE* file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not open file (reading: %s).\n", argv[1]);

        return QUIT(1);
    }
    if (argv[2] != NULL && (!strcmp(argv[2], "-d") || !strcmp(argv[2], "--debug")))
    {
        debug = 1;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read the file into a string
    char* contents = malloc(size);

    fread(contents, 1, size, file);
    contents[size] = '\0'; // null terminate

    fclose(file);


    // tokenise the string
    Token* tokens = NULL;
    tokenise(&tokens, contents, size);


    // parse the tokens
    Node root = parse(contents, tokens, 0, getTokenAmount(tokens)-1, NODE_PROGRAM);

    if (debug) {
        printf("CONTENTS:\n\n");
        for (int i = 0; i < size + 10; i++) {
            printf("%c", contents[i] == '\0' ? '!' : contents[i]);
        }
        printf("\n\n\n\nTOKENS:\n\n");
        printTokens(tokens);

        printf("\n\n\nAST:\n\n");
        printNode(&root, 1, 1);
    }

    destroyNode(&root);

    free(tokens);
    free(contents);

    return QUIT(0);
}


int QUIT (int code)
{
    return code;
}
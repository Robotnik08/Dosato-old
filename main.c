#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/str_tools.h"
#include "includes/lang.h"
#include "includes/lexer.h"
#include "includes/parser.h"

// forward declarations
int QUIT (int code);



int main (int argc, char* argv[])
{
    if (argc != 2)
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
        
        return QUIT(0);
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("Dosato Version: 0.0.1\n");
        printf("2023, made by Sebastiaan Heins\n");

        return QUIT(0);
    }

    FILE* file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not open file (reading: %s).\n", argv[1]);

        return QUIT(1);
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
    // printTokens(tokens);

    // parse the tokens
    Node root = parse(tokens, getTokenAmount(tokens));

    printNode(&root, 1, 1);

    free(tokens);
    free(contents);

    return QUIT(0);
}


int QUIT (int code)
{
    return code;
}
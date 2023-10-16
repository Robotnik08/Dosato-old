/**
 * @author Sebastiaan Heins
 * @file main.c
 * @brief The main file and entry point of the Dosato interpreter
 * @version 0.0.3
 * @date 05-10-2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// includes
#include "includes/str_tools.h"
#include "includes/lexer.h"
#include "includes/parser.h"
#include "includes/log.h"
#include "includes/ast_debug.h"
#include "includes/ast.h"

// forward declarations
/**
 * @brief Quit the program with a code
 * @param code The code to quit with
*/
int QUIT (int code);

// global variables
int debug = 0;

int main (int argc, char* argv[])
{
    // if no arguments are given, show hint
    if (argc < 2)
    {
        printf("Use -h or -help for help\n");
        return QUIT(0);
    }

    // if the first argument is -h or --help, show help
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("Commands:\n");
        printf("\t-h, --help: Show this help message\n");
        printf("\t-v, --version: Show the version number\n");
        printf("\t(PROGRAM_NAME): Run a file\n");
        printf("\t(PROGRAM_NAME) -d, --debug: Run a file in debug mode\n");
        
        return QUIT(0);
    }

    // if the first argument is -v or --version, show the version number
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("Dosato Version: 0.0.3\n");
        printf("2023, made by Sebastiaan Heins\n");

        return QUIT(0);
    }

    // open the file
    FILE* file = fopen(argv[1], "r");

    // if the file could not be opened, show an error
    if (file == NULL)
    {
        printf("Could not open file (reading: %s).\n", argv[1]);
        return QUIT(1);
    }
    
    // check if the file should be run in debug mode
    debug = argv[2] != NULL && (!strcmp(argv[2], "-d") || !strcmp(argv[2], "--debug"));

    // get the size of the file
    int size = 0;
    while (fgetc(file) != EOF) {
        size++;
    }
    fseek(file, 0, SEEK_SET);

    // read the file into a string
    char* contents = malloc(size);
    contents[size] = '\0';
    fread(contents, 1, size, file);

    // close the file
    fclose(file);

    // the main module, containing the entry point of the program
    AST main;
    main.full_code = malloc (size);
    strcpy(main.full_code, contents);

    main.filename = argv[1];

    // the modules, containing the code of the modules, TO DO (modules are not implemented yet)
    AST* modules = NULL;
    int module_amount = 0;

    /// STEP 1: LEXER ///

    // tokenise the string
    main.tokens = NULL;
    tokenise(&main.tokens, main.full_code, size);

    /// STEP 2: PARSER ///

    // parse the tokens, starting at the root node, containing the entire program
    main.root = parse(main.full_code, main.tokens, 0, getTokenAmount(main.tokens)-1, NODE_PROGRAM);

    /// DEBUG ///
    if (debug) {
        printf("CONTENTS (%s):\n\n", main.filename);
        for (int i = 0; i < size; i++) {
            printf("%c", main.full_code[i]);
        }
        printf("\n\n\n\nTOKENS:\n\n");
        printTokens(main.tokens);

        printf("\n\n\nAST:\n\n");
        printNode(main.full_code, main.tokens, &main.root, 1, 1);
    }

    /// STEP end: CLEANUP ///
    destroyNode(&main.root);

    free(main.tokens);
    free(contents);
    free(main.full_code);

    // flawless execution
    return QUIT(0);
}

int QUIT (int code)
{
    return code;
}
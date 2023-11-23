/**
 * @author Sebastiaan Heins
 * @file main.c
 * @brief The main file and entry point of the Dosato interpreter
 * @version 0.2
 * @date 05-10-2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// includes
#include "includes/strtools.h"
#include "includes/lexer.h"
#include "includes/parser.h"
#include "includes/log.h"
#include "includes/ast_debug.h"
#include "includes/ast.h"
#include "includes/process.h"

#define VERSION "0.9"
#define PROGRAM_NAME "Dosato"
#define AUTHOR "Sebastiaan Heins"

// forward declarations
/**
 * @brief Quit the program with a code
 * @param code The code to quit with
*/
int QUIT (int code);


// global variables
int debug = 0;

int main (int argc, char** argv)
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
        printf("%s Version: %s\n", PROGRAM_NAME, VERSION);
        printf("2023, made by %s\n", AUTHOR);

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
    long long int size = 0;
    while (fgetc(file) != EOF) {
        size++;
    }
    fseek(file, 0, SEEK_SET);

    // read the file into a string
    char* contents = malloc(size + 1);
    fread(contents, 1, size, file);
    contents[size] = '\0';

    // close the file
    fclose(file);
    if (size == 0) {
        free(contents);
        return QUIT(0);
    }

    // the main process, containing the entry point of the program
    Process main = createProcess(debug, 1, loadAST(argv[1], contents));

    free(contents);
    /// DEBUG ///
    if (debug) {
        printf("CONTENTS (%s):\n\n", main.code[0].filename);
        for (int i = 0; i < size; i++) {
            printf("%c",main.code[0].full_code[i]);
        }
        printf("\n\n\n\nTOKENS:\n\n");
        printTokens(main.code[0].tokens);

        printf("\n\n\nAST:\n\n");
        printNode(main.code[0].full_code, main.code[0].tokens, &main.code[0].root, 1, 1);

        printf("\n\nRUNNING PROGRAM:\n\n");
    }

    /// RUN THE PROGRAM ///
    int exit_code = runProcess(&main);


    /// CLEANUP ///
    destroyProcess(&main);
    // flawless execution

    return QUIT(exit_code);
}

int QUIT (int code)
{
    if (debug) {
        printf("\n\nFinished with exit code %i\n", code);
    }
    _fcloseall(); // close all files if any were opened
    return code;
}
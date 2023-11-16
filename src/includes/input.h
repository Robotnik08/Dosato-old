/**
 * @author Sebastiaan Heins
 * @file variable.h
 * @brief Input handling keyboard (simple)
 * @version 0.6
 * @date 15-11-2023
*/

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Get input from the stdin (no buffer)
 * @param prompt The prompt to display before the input
 * @return The pointer to the input (make sure to free)
*/
char* getInput ();

char* getInput () {
    char* input = malloc(1);
    input[0] = '\0';
    char c;
    while ((c = getchar()) != '\n') {
        input = realloc(input, strlen(input) + 2);
        input[strlen(input) + 1] = '\0';
        input[strlen(input)] = c;
    }
    return input;
}


#endif
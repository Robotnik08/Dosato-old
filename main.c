#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/str_tools.h"
#include "includes/lang.h"

// forward declarations
int QUIT (int code);



int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main <filename>\n");
        return QUIT(1);
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
    char* contents = malloc(size);
    fread(contents, 1, size, file);
    fclose(file);
    char** lines = str_split(contents, ";");
    free(contents);

    for (int i = 0; lines[i] != NULL; i++)
    {
        str_replace(lines[i], "\n", " ");
        str_replace(lines[i], "\t", " ");
    }

    run(lines, sizeof(lines)/sizeof(char*));

    for (int i = 0; lines[i] != NULL; i++)
    {
        free(lines[i]);
    }
    free(lines);

    return QUIT(1);
}


int QUIT (int code)
{
    if (code) system("pause");
    return code;
}
#ifndef STR_TOOLS_H
#define STR_TOOLS_H

#include <string.h>

char** str_split(const char* input, const char* separator) {
    int count = 1;
    const char* temp = input;
    while ((temp = strstr(temp, separator))) {
        count++;
        temp += strlen(separator);
    }

    char** substrings = (char**)malloc((count + 1) * sizeof(char*));

    const char* start = input;
    int i = 0;

    while (1) {
        const char* end = strstr(start, separator);
        if (end == NULL) {
            substrings[i] = (char*)malloc(strlen(start) + 1);
            strcpy(substrings[i], start);
            break;
        }

        int length = end - start;
        substrings[i] = (char*)malloc(length + 1);
        strncpy(substrings[i], start, length);
        substrings[i][length] = '\0';

        start = end + strlen(separator);
        i++;
    }

    substrings[count] = NULL;

    return substrings;
}

void str_replace(char *in, const char *selector, const char *replacement) {
    int in_len = strlen(in);
    int selector_len = strlen(selector);

    char *result = (char *)malloc(in_len * 2);

    result[0] = '\0';

    for (int i = 0; i < in_len; ) {
        if (strncmp(in + i, selector, selector_len) == 0) {
            strcat(result, replacement);
            i += selector_len;
        } else {
            strncat(result, in + i, 1);
            i++;
        }
    }

    strcpy(in, result);

    free(result);
}

#endif
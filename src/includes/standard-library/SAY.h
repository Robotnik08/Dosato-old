#ifndef SAY_H
#define SAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ast.h"
#include "../parser.h"
#include "../lexer.h"
#include "../error.h"
#include "../function.h"
#include "../variable.h"
#include "../token.h"
#include "../scope.h"
#include "../process.h"
#include "../log.h"


int std_SAY (Process* process, const Variable* args, int argc, int newline);

int printVar (const Variable* var);

int std_SAY (Process* process, const Variable* args, int argc, int newline) {
    int code = 0;
    for (int i = 0; i < argc; i++) {
        printVar(&args[i]);
    }
    if (newline) {
        code |= printf("\n") < 0 ? ERROR_SAY_ERROR : 0;
    }
    return code;
}

int printVar (const Variable* var) {
    int code = 0;
    if (var->type.array) {
        int len = getVariablesLength(var->value);
        code |= printf("[") < 0 ? ERROR_SAY_ERROR : 0;
        for (int i = 0; i < len; i++) {
            if (compareType(var->type, (Type){TYPE_STRING, 1})) printf("\"");
            if (compareType(var->type, (Type){TYPE_CHAR, 1})) printf("'");
            code |= printVar(&((Variable*)var->value)[i]) < 0 ? ERROR_SAY_ERROR : 0;
            if (compareType(var->type, (Type){TYPE_STRING, 1})) printf("\"");
            if (compareType(var->type, (Type){TYPE_CHAR, 1})) printf("'");
            if (i < len - 1) code |= printf(", ") < 0 ? ERROR_SAY_ERROR : 0;
        }
        code |= printf("]") < 0 ? ERROR_SAY_ERROR : 0;
        return code;
    }
    switch (var->type.dataType)
    {
        default:
            return ERROR_TYPE_MISMATCH;
            break;
        case TYPE_CHAR: {}
            code |= printf("%c", *(char*)(var->value)) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_STRING: {}
            code |= printf("%s", (char*)var->value) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_BYTE: {}
            signed char* cvalue = (signed char*)var->value;
            code |= printf("%i", *cvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_SHORT: {}
            short int* svalue = (short int*)var->value;
            code |= printf("%hi", *svalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_INT: {}
            int* ivalue = (int*)var->value;
            code |= printf("%i", *ivalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_LONG: {}
            long long int* lvalue = (long long int*)var->value;
            code |= printf("%lld", *lvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_UBYTE: {}
            unsigned char* ucvalue = (unsigned char*)var->value;
            code |= printf("%u", *ucvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_USHORT: {}
            unsigned short int* usvalue = (unsigned short int*)var->value;
            code |= printf("%hu", *usvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_UINT: {}
            unsigned int* uivalue = (unsigned int*)var->value;
            code |= printf("%u", *uivalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_ULONG: {}
            unsigned long long int* uvalue = (unsigned long long int*)var->value;
            code |= printf("%llu", *uvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_FLOAT: {}
            float* fvalue = (float*)var->value;
            code |= printf("%.9g", *fvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_DOUBLE: {}
            double* dvalue = (double*)var->value;
            code |= printf("%.17g", *dvalue) < 0 ? ERROR_SAY_ERROR : 0;
            break;
        case TYPE_BOOL: {}
            int* bvalue = (int*)var->value;
            code |= printf("%s", *bvalue ? "TRUE" : "FALSE") < 0 ? ERROR_SAY_ERROR : 0;
            break;
    }
    return code;
}


#endif
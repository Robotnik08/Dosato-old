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

int say (Process* process, const Variable* args, int argc);

int say (Process* process, const Variable* args, int argc) {
    int code = 0;
    for (int i = 0; i < argc; i++) {
        switch (args[i].type)
        {
            default:
                return ERROR_TYPE_MISMATCH;
                break;
            case TYPE_CHAR:
                code = !code ? (printf("%c", *(char*)(args[i].value)) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_STRING:
                code = !code ? (printf("%s", args[i].value) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_BYTE:
                signed char* cvalue = (signed char*)args[i].value;
                code = !code ? (printf("%i", *cvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_SHORT:
                short int* svalue = (short int*)args[i].value;
                code = !code ? (printf("%hi", *svalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_INT:
                int* ivalue = (int*)args[i].value;
                code = !code ? (printf("%i", *ivalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_LONG:
                long long int* lvalue = (long long int*)args[i].value;
                code = !code ? (printf("%lld", *lvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_UBYTE:
                unsigned char* ucvalue = (unsigned char*)args[i].value;
                code = !code ? (printf("%u", *ucvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_USHORT:
                unsigned short int* usvalue = (unsigned short int*)args[i].value;
                code = !code ? (printf("%hu", *usvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_UINT:
                unsigned int* uivalue = (unsigned int*)args[i].value;
                code = !code ? (printf("%u", *uivalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_ULONG:
                unsigned long long int* uvalue = (unsigned long long int*)args[i].value;
                code = !code ? (printf("%llu", *uvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_FLOAT:
                float* fvalue = (float*)args[i].value;
                code = !code ? (printf("%f", *fvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_DOUBLE:
                double* dvalue = (double*)args[i].value;
                code = !code ? (printf("%lf", *dvalue) < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
            case TYPE_BOOL:
                int* bvalue = (int*)args[i].value;
                code = !code ? (printf("%s", *bvalue ? "TRUE" : "FALSE") < 0 ? ERROR_SAY_ERROR : 0) : code;
                break;
        }
    }
    return code;
}


#endif
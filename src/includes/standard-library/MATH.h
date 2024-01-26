#ifndef DOSATO_MATH_H
#define DOSATO_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include <limits.h>

#ifndef LONG_LONG_MIN
#define LONG_LONG_MIN -9223372036854775807
#endif
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX 9223372036854775807
#endif

#include "../ast.h"
#include "../parser.h"
#include "../lexer.h"
#include "../error.h"
#include "../function.h"
#include "../variable.h"
#include "../token.h"
#include "../scope.h"
#include "../process.h"

int std_SQRT (Process* process, const Variable* args, int argc);

int std_ABS (Process* process, const Variable* args, int argc);

int std_ROUND (Process* process, const Variable* args, int argc);

int std_FLOOR (Process* process, const Variable* args, int argc);

int std_CEIL (Process* process, const Variable* args, int argc);

int std_POW (Process* process, const Variable* args, int argc);

int std_MIN (Process* process, const Variable* args, int argc);

int std_MAX (Process* process, const Variable* args, int argc);

int std_LOG (Process* process, const Variable* args, int argc);

int std_LOG10 (Process* process, const Variable* args, int argc);

int std_SIN (Process* process, const Variable* args, int argc);

int std_COS (Process* process, const Variable* args, int argc);

int std_TAN (Process* process, const Variable* args, int argc);

int std_ASIN (Process* process, const Variable* args, int argc);

int std_ACOS (Process* process, const Variable* args, int argc);

int std_ATAN (Process* process, const Variable* args, int argc);

int std_ATAN2 (Process* process, const Variable* args, int argc);

int std_EXP (Process* process, const Variable* args, int argc);

int std_QUADRATIC (Process* process, const Variable* args, int argc);

int std_SQRT (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        if (in_val < 0) return ERROR_MATH_DOMAIN_ERROR;

        double* value = malloc(sizeof(long long int));
        *value = sqrt(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        if (in_val < 0) return ERROR_MATH_DOMAIN_ERROR;
        double* value = malloc(sizeof(double));
        *value = sqrt(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}


int std_ABS (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        long long int* value = malloc(sizeof(long long int));
        *value = llabs(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_LONG, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = fabs(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}

int std_ROUND (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = round(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = round(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}

int std_FLOOR (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = floor(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = floor(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}

int std_CEIL (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = ceil(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = ceil(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}

int std_POW (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        long long int in_val2 = getSignedNumber((Variable*)&args[1]);
        
        double* value = malloc(sizeof(long long int));
        *value = pow(in_val, in_val2);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        double in_val2 = getFloatNumber((Variable*)&args[1]);
        
        double* value = malloc(sizeof(double));
        *value = pow(in_val, in_val2);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }

    return 0; // return code
}

int std_MIN (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (argc == 1) {
        if (args[0].type.array) {
            if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType)) {
                int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 1});
                
                long long int* value = malloc(sizeof(long long int));

                int arr_len = getVariablesLength((Variable*)args[0].value);

                long long int min = LONG_LONG_MAX;
                for (int i = 1; i < arr_len; i++) {
                    long long int val = getSignedNumber(&((Variable*)args[0].value)[i]);
                    if (val < min) min = val;
                }

                *value = min;

                Variable* var = malloc(sizeof(Variable));
                *var = createVariable("-lit", TYPE_LONG, value, 0, 0);
                setReturnValue(process, var);

                destroyVariable(var);
                free(var);
            } else {
                int cRes = castValue((Variable*)&args[0], (Type){TYPE_DOUBLE, 1});
                
                double* value = malloc(sizeof(double));

                int arr_len = getVariablesLength((Variable*)args[0].value);

                double min = DBL_MAX;
                for (int i = 1; i < arr_len; i++) {
                    double val = getFloatNumber(&((Variable*)&args[0].value)[i]);
                    if (val < min) min = val;
                }

                *value = min;
                
                Variable* var = malloc(sizeof(Variable));
                *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
                setReturnValue(process, var);

                destroyVariable(var);
                free(var);
            }
        } else {
            return ERROR_TYPE_MISMATCH;
        }
    } else {
        if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType)) {
            long long int in_val = getSignedNumber((Variable*)&args[0]);
            long long int in_val2 = getSignedNumber((Variable*)&args[1]);
            
            long long int* value = malloc(sizeof(long long int));
            *value = in_val < in_val2 ? in_val : in_val2;

            Variable* var = malloc(sizeof(Variable));
            *var = createVariable("-lit", TYPE_LONG, value, 0, 0);
            setReturnValue(process, var);

            destroyVariable(var);
            free(var);
        } else {
            double in_val = getFloatNumber((Variable*)&args[0]);
            double in_val2 = getFloatNumber((Variable*)&args[1]);
            
            double* value = malloc(sizeof(double));
            *value = in_val < in_val2 ? in_val : in_val2;
            
            Variable* var = malloc(sizeof(Variable));
            *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
            setReturnValue(process, var);

            destroyVariable(var);
            free(var);
        }
    }
    return 0; // return code
}

int std_MAX (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 1) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    if (argc == 1) {
        if (args[0].type.array) {
            if (!checkIfFloating(args[0].type.dataType)) {
                int cRes = castValue((Variable*)&args[0], (Type){TYPE_LONG, 1});
                if (cRes) return cRes;
                
                long long int* value = malloc(sizeof(long long int));

                int arr_len = getVariablesLength((Variable*)args[0].value);

                long long int max = LONG_LONG_MIN;
                for (int i = 0; i < arr_len; i++) {
                    long long int val = getSignedNumber(&((Variable*)args[0].value)[i]);
                    if (val > max) max = val;
                }

                *value = max;

                Variable* var = malloc(sizeof(Variable));
                *var = createVariable("-lit", TYPE_LONG, value, 0, 0);
                setReturnValue(process, var);

                destroyVariable(var);
                free(var);
            } else {
                int cRes = castValue((Variable*)&args[0], (Type){TYPE_DOUBLE, 1});
                if (cRes) return cRes;
                
                double* value = malloc(sizeof(double));

                int arr_len = getVariablesLength((Variable*)args[0].value);

                double max = DBL_MIN;
                for (int i = 0; i < arr_len; i++) {
                    double val = getFloatNumber(&((Variable*)args[0].value)[i]);
                    if (val > max) max = val;
                }
                
                *value = max;
                
                Variable* var = malloc(sizeof(Variable));
                *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
                setReturnValue(process, var);

                destroyVariable(var);
                free(var);
            }
        } else {
            return ERROR_TYPE_MISMATCH;
        }
    } else {
        if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType)) {
            long long int in_val = getSignedNumber((Variable*)&args[0]);
            long long int in_val2 = getSignedNumber((Variable*)&args[1]);
            
            long long int* value = malloc(sizeof(long long int));
            *value = in_val > in_val2 ? in_val : in_val2;

            Variable* var = malloc(sizeof(Variable));
            *var = createVariable("-lit", TYPE_LONG, value, 0, 0);
            setReturnValue(process, var);

            destroyVariable(var);
            free(var);
        } else {
            double in_val = getFloatNumber((Variable*)&args[0]);
            double in_val2 = getFloatNumber((Variable*)&args[1]);
            
            double* value = malloc(sizeof(double));
            *value = in_val > in_val2 ? in_val : in_val2;
            
            Variable* var = malloc(sizeof(Variable));
            *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
            setReturnValue(process, var);

            destroyVariable(var);
            free(var);
        }
    }
    return 0; // return code
}

int std_LOG (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = log(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = log(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_LOG10 (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = log10(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = log10(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_SIN (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = sin(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = sin(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);

        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_COS (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = cos(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = cos(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_TAN (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = tan(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = tan(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_ASIN (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        if (in_val < -1 || in_val > 1) return ERROR_MATH_DOMAIN_ERROR;
        
        double* value = malloc(sizeof(long long int));
        *value = asin(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        if (in_val < -1 || in_val > 1) return ERROR_MATH_DOMAIN_ERROR;
        
        double* value = malloc(sizeof(double));
        *value = asin(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_ACOS (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        if (in_val < -1 || in_val > 1) return ERROR_MATH_DOMAIN_ERROR;
        
        double* value = malloc(sizeof(long long int));
        *value = acos(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        if (in_val < -1 || in_val > 1) return ERROR_MATH_DOMAIN_ERROR;
        
        double* value = malloc(sizeof(double));
        *value = acos(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_ATAN (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = atan(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = atan(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_ATAN2 (Process* process, const Variable* args, int argc) {
    if (argc > 2) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 2) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        long long int in_val2 = getSignedNumber((Variable*)&args[1]);
        
        double* value = malloc(sizeof(long long int));
        *value = atan2(in_val, in_val2);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        double in_val2 = getFloatNumber((Variable*)&args[1]);
        
        double* value = malloc(sizeof(double));
        *value = atan2(in_val, in_val2);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_EXP (Process* process, const Variable* args, int argc) {
    if (argc > 1) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType)) {
        long long int in_val = getSignedNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(long long int));
        *value = exp(in_val);

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double in_val = getFloatNumber((Variable*)&args[0]);
        
        double* value = malloc(sizeof(double));
        *value = exp(in_val);
        
        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 0);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

int std_QUADRATIC (Process* process, const Variable* args, int argc) {
    if (argc > 3) {
        return ERROR_TOO_MANY_ARGUMENTS;
    }
    if (argc < 3) {
        return ERROR_TOO_FEW_ARGUMENTS;
    }

    if (!checkIfFloating(args[0].type.dataType) && !checkIfFloating(args[1].type.dataType) && !checkIfFloating(args[2].type.dataType)) {
        long long int a = getSignedNumber((Variable*)&args[0]);
        long long int b = getSignedNumber((Variable*)&args[1]);
        long long int c = getSignedNumber((Variable*)&args[2]);
        
        Variable* value = malloc(sizeof(Variable) * 3); // Array of 2 variables

        Variable* var1 = malloc(sizeof(Variable));
        Variable* var2 = malloc(sizeof(Variable));

        double* value1 = malloc(sizeof(double));
        double* value2 = malloc(sizeof(double));

        *value1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        *value2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);

        *var1 = createVariable("-lit", TYPE_DOUBLE, value1, 0, 0);
        *var2 = createVariable("-lit", TYPE_DOUBLE, value2, 0, 0);

        ((Variable*)value)[0] = *var1;
        ((Variable*)value)[1] = *var2;
        ((Variable*)value)[2] = createNullTerminatedVariable();

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 1);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    } else {
        double a = getFloatNumber((Variable*)&args[0]);
        double b = getFloatNumber((Variable*)&args[1]);
        double c = getFloatNumber((Variable*)&args[2]);

        Variable* value = malloc(sizeof(Variable) * 3); // Array of 2 variables
        
        Variable* var1 = malloc(sizeof(Variable));
        Variable* var2 = malloc(sizeof(Variable));

        double* value1 = malloc(sizeof(double));
        double* value2 = malloc(sizeof(double));

        *value1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        *value2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);

        *var1 = createVariable("-lit", TYPE_DOUBLE, value1, 0, 0);
        *var2 = createVariable("-lit", TYPE_DOUBLE, value2, 0, 0);

        ((Variable*)value)[0] = *var1;
        ((Variable*)value)[1] = *var2;
        ((Variable*)value)[2] = createNullTerminatedVariable();

        Variable* var = malloc(sizeof(Variable));
        *var = createVariable("-lit", TYPE_DOUBLE, value, 0, 1);
        
        setReturnValue(process, var);
        
        destroyVariable(var);
        free(var);
    }
    return 0;
}

#endif
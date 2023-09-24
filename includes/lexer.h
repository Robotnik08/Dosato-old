#ifndef LEXER_H
#define LEXER_H

#include "str_tools.h"
#include "token.h"

#define MASTER_KEYWORDS {"DO", "MAKE", "SET"}
#define KEYWORDS {"WHEN", "WHILE", "ELSE", "ELSEWHEN", "CATCH", "STORE", "THEN"}
#define VAR_TYPES {"INT", "BOOL", "STRING", "FLOAT", "DOUBLE", "CHAR", "SHORT", "LONG", "BYTE", "VOID", "ARRAY", "FUNC"}
#define SEPARATORS {';'}
#define OPERATORS {"+", "-", "*", "/", "%", "=", ">", "<", "!", "&", "|", "^", "~", "?", ":", ".", ",", "#", \
                    "+=", "-=", "*=", "/=", "%=", "++", "--", "==", "!=", ">=", "<=", "&&", "||", "<<", ">>"}
#define BRACKETS {"()", "{}", "[]"}



void tokenise (Token** tokens, const char* full_code, const int code_length);
void addToken(Token** tokensPtr, TokenType type, int start, int end, int carry) {
    int numTokens = 0;
    if (*tokensPtr) {
        // Find the number of tokens by searching for the sentinel token.
        while ((*tokensPtr)[numTokens].type != TOKEN_END) {
            numTokens++;
        }
    }
    *tokensPtr = realloc(*tokensPtr, (numTokens + 2) * sizeof(Token));

    (*tokensPtr)[numTokens].start = start;
    (*tokensPtr)[numTokens].end = end;
    (*tokensPtr)[numTokens].type = type;
    (*tokensPtr)[numTokens].carry = carry;

    // Add a sentinel token to mark the end of the array.
    (*tokensPtr)[numTokens + 1].type = TOKEN_END;
}
int getTokenAmount (Token* tokens) {
    if (!tokens) return 0;
    int amount = 0;
    while (tokens[amount].type != TOKEN_END) {
        amount++;
    }
    return amount;
}
void printTokens (Token* tokens);

void tokenise (Token** tokens, const char* full_code, const int code_length) {

    int start = 0;
    int end = 0;
    char quotationtype = '\0';
    int escapeCount = 0;
    int inComment = 0;

    int tokenCount = 0;

    // get comment and string tokens
    for (int i = 0; i < code_length; i++) {
        if (quotationtype == '\0') {
            if ((full_code[i] == '"' || full_code[i] == '\'') && escapeCount % 2 == 0) {
                quotationtype = full_code[i];
                start = i;
                continue;
            }
            if (full_code[i] == '/' && full_code[i + 1] == '/' && !inComment) {
                int foundEnd = 0;
                start = i;
                for (int j = i; j < code_length; j++) {
                    if (full_code[j] == '\n') {
                        end = j - 1;
                        addToken(tokens, TOKEN_COMMENT, start, end, 0);
                        start = end + 1;
                        i = j;
                        foundEnd = 1;
                        break;
                    }
                }
                if (!foundEnd) {
                    end = code_length - 1;
                    addToken(tokens, TOKEN_COMMENT, start, end, 0);
                    start = end + 1;
                    i = code_length;
                }
            }
        } else {
            if (full_code[i] == quotationtype && escapeCount % 2 == 0) {
                end = i;
                addToken(tokens, TOKEN_STRING, start, end, 0);
                start = end + 1;
                quotationtype = '\0';
            }
        }

        // Check for escape character
        if (full_code[i] == '\\') {
            escapeCount++;
        } else {
            escapeCount = 0;
        }
    }
    
    tokenCount = getTokenAmount(*tokens); // update token count with the string and comment tokens
    
    // get master tokens
    const char* mastertokens[] = MASTER_KEYWORDS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        for (int j = 0; j < sizeof(mastertokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, mastertokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_MASTER_KEYWORD, i, i + strlen(mastertokens[j]) - 1, 0);
                i += strlen(mastertokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }

    // get var_type tokens
    const char* var_typetokens[] = VAR_TYPES;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        for (int j = 0; j < sizeof(var_typetokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, var_typetokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_TYPE_KEYWORD, i, i + strlen(var_typetokens[j]) - 1, 0);
                i += strlen(var_typetokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }

    // get extension tokens
    const char* extension_tokens[] = KEYWORDS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        for (int j = 0; j < sizeof(extension_tokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, extension_tokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_KEYWORD, i, i + strlen(extension_tokens[j]) - 1, 0);
                i += strlen(extension_tokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }
    
    // get bracket tokens
    const char* brackettokens[] = BRACKETS;
    int bracketTier = 0;
    int bracketTypeHiarcy[code_length];
    for (int k = 0; k < code_length; k++) bracketTypeHiarcy[k] = -1;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        // check for opening brackets
        for (int j = 0; j < sizeof(brackettokens)/sizeof(char*); j++) {
            if (full_code[i] == brackettokens[j][0]) {
                bracketTier++;
                addToken(tokens, TOKEN_PARENTHESIS, i, i, bracketTier);
                bracketTypeHiarcy[bracketTier - 1] = j;
                break;
            }
        }
        // check for closing brackets
        for (int j = 0; j < sizeof(brackettokens)/sizeof(char*); j++) {
            if (full_code[i] == brackettokens[j][1]) {
                addToken(tokens, TOKEN_PARENTHESIS, i, i, bracketTypeHiarcy[bracketTier - 1] == j ? bracketTier : -1);
                if (bracketTypeHiarcy[bracketTier - 1] == j) {
                    bracketTypeHiarcy[bracketTier - 1] = -1;
                    bracketTier--;
                }
                break;
            }
        }
    }

    // get separator tokens
    const char separatortokens[] = SEPARATORS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        for (int j = 0; j < sizeof(separatortokens); j++) {
            if (full_code[i] == separatortokens[j]) {
                addToken(tokens, TOKEN_SEPARATOR, i, i, 0);
                break;
            }
        }
    }

    // getNumber tokens
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        if (isNumeric(full_code[i])) {
            if (full_code[i] == '.') {
                if (!isNumeric(full_code[i + 1])) continue;
            }
            int start = i;
            int end = i;
            int foundDecimal = 0;

            int invalid = 0;
            for (int j = i; j < code_length; j++) {
                if (j && !isNumeric(full_code[j-1]) && !isEmpty(full_code[j-1])) {
                    invalid = 1;
                    for (int k = j; k < code_length && isNumeric(full_code[k]); k++) {
                        end = k;
                    }
                    break;
                }
                if (isNumeric(full_code[j])) {
                    end = j;
                    if (full_code[j] == '.' && foundDecimal) break;
                } else if (full_code[j] == '.' && !foundDecimal) {
                    end = j;
                    foundDecimal = 1;
                } else {
                    break;
                }
            }
            if (!invalid) addToken(tokens, TOKEN_NUMBER, start, end, 0);
            i = end;
        }
    }

    // get operator tokens
    const char* operatortokens[] = OPERATORS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        int foundBig = 0;
        for (int j = 0; j < sizeof(operatortokens)/sizeof(char*); j++) {
            char* bigoperator = malloc(2);
            bigoperator[0] = full_code[i];
            bigoperator[1] = full_code[i+1];
            if (!strcmp(bigoperator, operatortokens[j])) {
                free(bigoperator);
                addToken(tokens, TOKEN_OPERATOR, i, i + strlen(operatortokens[j]) - 1, 0);
                i++;
                foundBig = 1;
                break;
            } else {
                free(bigoperator);
            }
        }
        if (foundBig) continue;
        for (int j = 0; j < sizeof(operatortokens)/sizeof(char*); j++) {
            if (full_code[i] == operatortokens[j][0]) {
                addToken(tokens, TOKEN_OPERATOR, i, i + strlen(operatortokens[j]) - 1, 0);
                break;
            }
        }
    }

    tokenCount = getTokenAmount(*tokens); // update token count to include all tokens so far
    
    // get general tokens (variables, functions, etc.)
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
                break;
            }
        }
        if (isAlphaNumeric(full_code[i])) {
            char* word = getWord(full_code, i);
            if (strlen(word) > 0) {
                addToken(tokens, TOKEN_IDENTIFIER, i, i + strlen(word) - 1, 0);
                i += strlen(word) - 1;
            }
            free(word);
        }
    }
}

void printTokens (Token* tokens) {
    if (tokens == NULL) {
        printf("No tokens.\n");
        return;
    }
    int i = 0;
    while (tokens[i].type != TOKEN_END) {
        printf("Token %d. start: %d, end: %d, type: %d, carry: %d\n",
               i, tokens[i].start, tokens[i].end, tokens[i].type, tokens[i].carry);
        i++;
    }
}
#endif
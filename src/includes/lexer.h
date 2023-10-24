/**
 * @author Sebastiaan Heins
 * @file lexer.h
 * @brief The lexer does a lexical analysis of the code and tokenises it, returning a list of tokens (defined in token.h)
 * @version 1.1
 * @date 05-10-2023
*/

#ifndef LEXER_H
#define LEXER_H

#include "strtools.h"
#include "token.h"

/**
 * @brief Tokenise a string of code
 * @param tokens The tokens to fill
 * @param full_code The full code to tokenise
 * @param code_length The length of the code
 * @warning The tokens must be freed after use
*/
void tokenise (Token** tokens, const char* full_code, const int code_length);

/**
 * @brief Add a token to a list of tokens
 * @param tokensPtr The list of tokens
 * @param type The type of the token
 * @param start The start index of the token
 * @param end The end index of the token
 * @param carry The carry of the token
*/
void addToken(Token** tokensPtr, TokenType type, int start, int end, int carry);

/**
 * @brief Get the amount of tokens in a list of tokens
 * @param tokens The list of tokens
 * @return The amount of tokens
*/
int getTokenAmount (Token* tokens);

/**
 * @brief Sort a list of tokens
 * @param tokens The list of tokens
*/
void sortTokens (Token** tokens);

/**
 * @brief Trim comments from a list of tokens
 * @param tokens The list of tokens
*/
void trimComments (Token** tokens);


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

void sortTokens (Token** tokens) {
    int tokenCount = getTokenAmount(*tokens);
    for (int i = 0; i < tokenCount; i++) {
        for (int j = 0; j < tokenCount - i - 1; j++) {
            if ((*tokens)[j].start > (*tokens)[j + 1].start) {
                Token temp = (*tokens)[j];
                (*tokens)[j] = (*tokens)[j + 1];
                (*tokens)[j + 1] = temp;
            }
        }
    }
}

void trimComments (Token** tokens) {
    int tokenCount = getTokenAmount(*tokens);
    for (int i = 0; i < tokenCount; i++) {
        if ((*tokens)[i].type == TOKEN_COMMENT) {
            for (int j = i; j < tokenCount; j++) {
                (*tokens)[j] = (*tokens)[j + 1];
            }
            tokenCount--;
            i--;
        }
    }
    *tokens = realloc(*tokens, (tokenCount + 1) * sizeof(Token));
    (*tokens)[tokenCount].type = TOKEN_END;
}

void tokenise (Token** tokens, const char* full_code, const int code_length) {
    int tokenCount = 0;

    // get comment and string tokens
    int start = 0;
    int end = 0;
    char quotationtype = '\0';
    int escapeCount = 0;
    for (int i = 0; i < code_length; i++) {
        if (quotationtype == '\0') {
            if ((full_code[i] == '"' || full_code[i] == '\'') && escapeCount % 2 == 0) {
                quotationtype = full_code[i];
                start = i;
                continue;
            }
            if (full_code[i] == '/' && full_code[i + 1] == '/') {
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
    
    tokenCount = getTokenAmount(*tokens); // update token count to include all tokens so far
    sortTokens(tokens); // sort tokens so that they are in order
    
    // get master tokens
    const char* mastertokens[] = MASTER_KEYWORDS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        for (int j = 0; j < sizeof(mastertokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, mastertokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_MASTER_KEYWORD, i, i + strlen(mastertokens[j]) - 1, j);
                i += strlen(mastertokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }
    
    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);

    // get var_type tokens
    const char* var_typetokens[] = VAR_TYPES;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        for (int j = 0; j < sizeof(var_typetokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, var_typetokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_VAR_TYPE, i, i + strlen(var_typetokens[j]) - 1, j);
                i += strlen(var_typetokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }
    
    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);

    // get extension tokens
    const char* extension_tokens[] = EXTENSION_KEYWORDS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        for (int j = 0; j < sizeof(extension_tokens)/sizeof(char*); j++) {
            char* next_word = getWord(full_code, i);
            if (!strcmp(next_word, extension_tokens[j])) {
                free(next_word);
                addToken(tokens, TOKEN_EXT, i, i + strlen(extension_tokens[j]) - 1, j);
                i += strlen(extension_tokens[j]) - 1;
                break;
            } else {
                free(next_word);
            }
        }
    }
    
    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);
    
    // get bracket tokens
    const char* brackettokens[] = BRACKETS;
    int bracketTier = 0;
    int bracketTypeHiarcy[code_length];
    for (int k = 0; k < code_length; k++) bracketTypeHiarcy[k] = -1;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        // check for opening brackets
        for (int j = 0; j < sizeof(brackettokens)/sizeof(char*); j++) {
            if (full_code[i] == brackettokens[j][0]) {
                bracketTier++;
                addToken(tokens, TOKEN_PARENTHESIS, i, i, getBracketType(full_code[i]) | bracketTier);
                bracketTypeHiarcy[bracketTier - 1] = j;
                break;
            }
        }
        // check for closing brackets
        for (int j = 0; j < sizeof(brackettokens)/sizeof(char*); j++) {
            if (full_code[i] == brackettokens[j][1]) {
                addToken(tokens, TOKEN_PARENTHESIS, i, i, getBracketType(full_code[i]) | (bracketTypeHiarcy[bracketTier - 1] == j ? bracketTier : -1));
                if (bracketTypeHiarcy[bracketTier - 1] == j) {
                    bracketTypeHiarcy[bracketTier - 1] = -1;
                    bracketTier--;
                }
                break;
            }
        }
    }

    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);

    // get separator tokens
    const char separatortokens[] = SEPARATORS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        for (int j = 0; j < sizeof(separatortokens); j++) {
            if (full_code[i] == separatortokens[j]) {
                addToken(tokens, TOKEN_SEPARATOR, i, i, 0);
                break;
            }
        }
    }
    
    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);

    // getNumber tokens
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        if (isFloateric(full_code[i])) {
            if (full_code[i] == '.') {
                if (!isFloateric(full_code[i + 1])) continue;
            }
            int start = i;
            int end = i;
            int foundDecimal = 0;

            int invalid = 0;
            for (int j = i; j < code_length; j++) {
                if (isAlphaNameric(full_code[j-1])) {
                    invalid = 1;
                    for (int k = j; k < code_length && isFloateric(full_code[k]); k++) {
                        end = k;
                    }
                    break;
                }
                if (isFloateric(full_code[j])) {
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
    
    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);

    // get operator tokens
    const char* operatortokens[] = OPERATORS;
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
            }
        }
        int foundBig = 0;
        for (int j = 0; j < sizeof(operatortokens)/sizeof(char*); j++) {
            char bigoperator[3] = { full_code[i], full_code[i+1], '\0' };
            if (!strcmp(bigoperator, operatortokens[j])) {
                addToken(tokens, TOKEN_OPERATOR, i, i + strlen(operatortokens[j]) - 1, j);
                i++;
                foundBig = 1;
                break;
            }
        }
        if (foundBig) continue;
        for (int j = 0; j < sizeof(operatortokens)/sizeof(char*); j++) {
            if (full_code[i] == operatortokens[j][0]) {
                addToken(tokens, TOKEN_OPERATOR, i, i + strlen(operatortokens[j]) - 1, j);
                break;
            }
        }
    }

    tokenCount = getTokenAmount(*tokens);
    sortTokens(tokens);
    
    // get general tokens (variables, functions, etc.)
    for (int i = 0; i < code_length; i++) {
        for (int t = 0; t < tokenCount; t++) {
            if (i == (*tokens)[t].start) {
                i = (*tokens)[t].end + 1;
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

    // finalise tokens
    sortTokens(tokens);
    trimComments(tokens); // comments will be ignored from now on
}
#endif
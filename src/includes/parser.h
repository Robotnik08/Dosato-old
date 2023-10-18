/**
 * @author Sebastiaan Heins
 * @file parser.h
 * @brief The parser is used to parse a list of tokens into an AST
 * @version 1.0
 * @date 05-10-2023
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"
#include "node.h"
#include "log.h"
#include "str_tools.h"

/**
 * @brief Parse a list of tokens into an AST
 * @param tokens The list of tokens to parse
 * @param start The index of the first token to parse
 * @param end The index of the last token to parse
 * @return The resulting AST from the list of tokens (don't forget to destroy it)
*/
Node parse (const char* code, Token* tokens, const int start, const int end, const NodeType type);

Node parse (const char* full_code, Token* tokens, const int start, const int end, const NodeType type) {
    Node root;
    root.start = start;
    root.end = end;
    root.type = type;
    root.body = NULL;
    switch (type) {
        // if the node is a program or a block, check for full lines of code
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = start; i < end; i++) {
                if (tokens[i].type == TOKEN_MASTER_KEYWORD) {
                    int full_line = getFullLine(tokens, i);
                    addToBody(&root.body, parse(full_code, tokens, i+1, full_line, NODE_FUNCTION_CALL + tokens[i].carry));
                    i = full_line;
                } else if (tokens[i].type != TOKEN_SEPARATOR) {
                    printError(full_code, tokens[i].start, ERROR_EXPECTED_MASTER);
                }
            }
            break;
        
        // when the DO keyword is the first keyword, handle function calling, this has multiple parts
        case NODE_FUNCTION_CALL:
            int got_identifier = 0;

            // loop through the tokens
            for (int i = start; i < end; i++) {
                // simple function call
                if (tokens[i].type == TOKEN_IDENTIFIER) {
                    if (got_identifier) printError(full_code, tokens[i].start, ERROR_EXPECTED_EXTENSION);
                    int args_end = getBlock(tokens, i+1);
                    addToBody(&root.body, parse(full_code, tokens, i, args_end, NODE_FUNCTION_IDENTIFIER));
                    i = args_end;
                    got_identifier = 1;
                // when the token is an extension keyword, check if it's valid
                } else if (tokens[i].type == TOKEN_EXT) {
                    int t_end = end;
                    const ExtensionKeywordType types[] = EXTENSION_ACCEPTS;
                    Node ext_root;
                    // check if the provided tokens match the extension
                    switch (types[tokens[i].carry])
                    {
                        // if the extension calls something, first check if it's a function identifier, then parse the arguments
                        case NEEDS_FUNCTION:
                            if (tokens[i+1].type == TOKEN_IDENTIFIER) {
                                if (tokens[i+2].type == TOKEN_PARENTHESIS && tokens[i+2].carry & BRACKET_ROUND) {
                                    t_end = getBlock(tokens, i+2);
                                    ext_root = parse(full_code, tokens, i, t_end, NODE_WHEN + tokens[i].carry);
                                    addToBody(&ext_root.body, parse(full_code, tokens, i+1, t_end, NODE_FUNCTION_IDENTIFIER));
                                    break;
                                }
                                printError(full_code, tokens[i+2].start, ERROR_EXPECTED_ARGUMENTS);
                            }
                        // if function not found search for a block instead (break missing on purpose)
                        case NEEDS_BLOCK:
                            if (tokens[i+1].type == TOKEN_PARENTHESIS && tokens[i+1].carry & BRACKET_CURLY) {
                                t_end = getBlock(tokens, i+1);
                                ext_root = parse(full_code, tokens, i, t_end, NODE_WHEN + tokens[i].carry);
                                addToBody (&ext_root.body, parse(full_code, tokens, i+2, t_end-1, NODE_BLOCK));
                            } else {
                                printError(full_code, tokens[i+1].start, ERROR_EXPECTED_BLOCK);
                            }
                            break;
                        // if the extension needs an expression, parse the expression
                        case NEEDS_EXPRESSION:
                            t_end = getExpression(tokens, i+1);
                            if (t_end != -1) {
                                ext_root = parse(full_code, tokens, i, t_end, NODE_WHEN + tokens[i].carry);
                                addToBody(&ext_root.body, parse(full_code, tokens, i+1, t_end, NODE_EXPRESSION));
                                break;
                            }
                            printError(full_code, tokens[i+1].start, ERROR_EXPECTED_EXPRESSION);
                    }
                    addToBody(&root.body, ext_root);
                    i = t_end;
                    got_identifier = 0;
                } else if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry & BRACKET_CURLY) {
                    if (got_identifier) printError(full_code, tokens[i].start, ERROR_EXPECTED_EXTENSION);
                    int t_end = getBlock(tokens, i);
                    addToBody(&root.body, parse(full_code, tokens, i+1, t_end-1, NODE_BLOCK));
                    i = t_end;
                    got_identifier = 1;
                } else {
                    printError(full_code, tokens[i].start, ERROR_EXPECTED_EXTENSION);
                }
            }
            break;
        // when the MAKE keyword is the first keyword, check for a type, an identifier and an expression
        case NODE_MAKE_VAR:
            // if the first token is not a type, throw an error
            if (tokens[start].type != TOKEN_VAR_TYPE) {
                printError(full_code, tokens[start].start, ERROR_EXPECTED_TYPE);
            }
            addToBody(&root.body, parse(full_code, tokens, start, start, NODE_TYPE_IDENTIFIER));


            // if the type is a func, parse the function declaration
            if (tokens[start].carry == TYPE_FUNC) {
                // change the node type to a function declaration instead of a variable declaration
                root.type = NODE_FUNCTION_DECLARATION;
                // if the second token is not an identifier, throw an error
                if (tokens[start + 1].type != TOKEN_IDENTIFIER) {
                    printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_IDENTIFIER);
                }
                // if theres no argument brackets, throw an error
                if (tokens[start + 2].type != TOKEN_PARENTHESIS || !(tokens[start + 2].carry & BRACKET_ROUND)) {
                    printError(full_code, tokens[start + 2].start, ERROR_EXPECTED_ARGUMENTS);
                }
                int args_end = getBlock(tokens, start + 2);
                addToBody(&root.body, parse(full_code, tokens, start + 2, args_end, NODE_FUNCTION_DECLARATION_ARGUMENTS));

                // if theres no block, throw an error
                if (tokens[args_end + 1].type != TOKEN_PARENTHESIS || !(tokens[args_end + 1].carry & BRACKET_CURLY)) {
                    printError(full_code, tokens[args_end + 1].start, ERROR_EXPECTED_BLOCK);
                }
                int block_end = getBlock(tokens, args_end + 1);
                // if the end of the block is not the end of the statement, throw an error
                if (block_end + 1 != end) {
                    printError(full_code, tokens[block_end + 1].start, ERROR_EXPECTED_SEPERATOR);
                }
                // if the block is empty, throw an error
                if (block_end - 2 - args_end <= 0) {
                    printError(full_code, tokens[args_end + 1].start, ERROR_EMPTY_BLOCK);
                } 
                addToBody(&root.body, parse(full_code, tokens, args_end + 2, block_end-1, NODE_BLOCK));
                break;
            } else if (tokens[start].carry == TYPE_ARRAY) {
                // after ARRAY, there should be a type identifier, if not throw an error
                if (tokens[start + 1].type != TOKEN_VAR_TYPE) {
                    printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_TYPE);
                }
                // parse the entire array declaration as a separate node
                addToBody(&root.body, parse(full_code, tokens, start + 1, end, NODE_MAKE_VAR));
                // change the node type to an array declaration instead of a variable declaration
                root.type = NODE_ARRAY_DECLARATION;
                break;
            }


            // if the second token is not an identifier, throw an error
            if (tokens[start + 1].type != TOKEN_IDENTIFIER) {
                printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_IDENTIFIER);
            }
            // check if the assignment operator is present, if not throw an error
            if (tokens[start + 2].type == TOKEN_OPERATOR && tokens[start + 2].carry == OPERATOR_ASSIGN) {
                addToBody(&root.body, parse(full_code, tokens, start + 1, start + 1, NODE_IDENTIFIER));
                addToBody(&root.body, parse(full_code, tokens, start + 3, end-1, NODE_EXPRESSION));
            } else {
                printError(full_code, tokens[start + 2].start, ERROR_EXPECTED_ASSIGN_OPERATOR);
            }
            break;
        // when a SET keyword is the first keyword, check for a variable (can be an expression), the operator and an expression
        case NODE_SET_VAR:
            int t_end = getSetExpression(tokens, start);
            if (t_end == -1) {
                printError(full_code, tokens[start].start, ERROR_EXPECTED_IDENTIFIER);
            }
            addToBody(&root.body, parse(full_code, tokens, start, t_end, NODE_EXPRESSION));
            if (tokens[t_end + 1].type == TOKEN_OPERATOR) {
                if (!isAssignmentOperator(tokens[t_end + 1].carry)) printError(full_code, tokens[t_end + 1].start, ERROR_EXPECTED_ASSIGN_OPERATOR);
                addToBody(&root.body, parse(full_code, tokens, t_end + 1, t_end + 1, NODE_OPERATOR));
            } else {
                printError(full_code, tokens[t_end + 1].start, ERROR_EXPECTED_ASSIGN_OPERATOR);
            }
            if (tokens[end].type != TOKEN_SEPARATOR) printError(full_code, tokens[end].start, ERROR_EXPECTED_SEPERATOR);
            addToBody(&root.body, parse(full_code, tokens, t_end + 2, end-1, NODE_EXPRESSION));
            break;
        // if the node is a function identifier, check if arguments are present
        case NODE_FUNCTION_IDENTIFIER:
            if (tokens[start].type != TOKEN_IDENTIFIER) {
                printError(full_code, tokens[start].start, ERROR_EXPECTED_IDENTIFIER);
            }
            if (tokens[start + 1].type == TOKEN_PARENTHESIS) {
                if (tokens[start + 1].carry & BRACKET_ROUND) {
                    addToBody(&root.body, parse(full_code, tokens, start + 1, getBlock(tokens, start + 1), NODE_ARGUMENTS));
                } else {
                    printError(full_code, tokens[start + 1].start, ERROR_WRONG_BRACKET_ROUND);
                }
            } else {
                printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_ARGUMENTS);
            }
            break;
        // if the node is a function call, split the arguments into separate expressions
        // OR if the node is an array expression, split the arguments into separate expressions
        case NODE_ARRAY_EXPRESSION:
        case NODE_ARGUMENTS:
            if (start == end-1) break; // if there are no arguments, don't parse anything
            int arg_start = start + 1;
            for (int i = start + 1; i < end - 1; i++) {
                if (tokens[i].type == TOKEN_OPERATOR && tokens[i].carry == OPERATOR_COMMA) {
                    addToBody(&root.body, parse(full_code, tokens, arg_start, i-1, NODE_EXPRESSION));
                    arg_start = i + 1;
                }
            }
            if (arg_start > end-1) printError(full_code, tokens[arg_start].start, ERROR_EXPECTED_ARGUMENT);
            addToBody(&root.body, parse(full_code, tokens, arg_start, end-1, NODE_EXPRESSION));
            break;
        // if the node is a function declaration, check if the arguments are valid, this is done by checking if a type and identifier are present
        case NODE_FUNCTION_DECLARATION_ARGUMENTS:
            if (start == end-1) break; // if there are no arguments, don't parse anything
            arg_start = start + 1;
            for (int i = start + 1; i < end - 1; i++) {
                if (tokens[i].type == TOKEN_OPERATOR && tokens[i].carry == OPERATOR_COMMA) {
                    addToBody(&root.body, parse(full_code, tokens, arg_start, i-1, NODE_FUNCTION_DECLARATION_ARGUMENT));
                    arg_start = i + 1;
                }
            }
            if (arg_start > end-1) printError(full_code, tokens[arg_start].start, ERROR_EXPECTED_ARGUMENT);
            addToBody(&root.body, parse(full_code, tokens, arg_start, end-1, NODE_FUNCTION_DECLARATION_ARGUMENT));
            break;
        // if the node is a function declaration argument, check if the type and identifier are valid
        case NODE_FUNCTION_DECLARATION_ARGUMENT:
            if (tokens[start].type == TOKEN_VAR_TYPE && tokens[start].carry == TYPE_ARRAY) {
                if (tokens[start + 1].type != TOKEN_VAR_TYPE) {
                    printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_TYPE);
                }
                addToBody(&root.body, parse(full_code, tokens, start, start, NODE_TYPE_IDENTIFIER));
                addToBody(&root.body, parse(full_code, tokens, start + 1, end, NODE_FUNCTION_DECLARATION_ARGUMENT));
                break;
            }
            if (!(end - start > 2 || end - start == 1)) printError(full_code, tokens[start].start, ERROR_INVALID_FUNCTION_DECLARATION_ARGUMENT);
            if (tokens[start].type != TOKEN_VAR_TYPE) {
                printError(full_code, tokens[start].start, ERROR_EXPECTED_TYPE);
            }
            if (tokens[start + 1].type != TOKEN_IDENTIFIER) {
                printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_IDENTIFIER);
            }
            addToBody(&root.body, parse(full_code, tokens, start, start, NODE_TYPE_IDENTIFIER));
            addToBody(&root.body, parse(full_code, tokens, start + 1, start + 1, NODE_IDENTIFIER));
            if (end - start > 2) {
                if (tokens[start + 2].type != TOKEN_OPERATOR || tokens[start + 2].carry != OPERATOR_ASSIGN) {
                    printError(full_code, tokens[start + 2].start, ERROR_EXPECTED_ASSIGN_OPERATOR);
                }
                addToBody(&root.body, parse(full_code, tokens, start + 2, start + 2, NODE_OPERATOR));
                addToBody(&root.body, parse(full_code, tokens, start + 3, end, NODE_EXPRESSION));
            }
            break;
        // binary expressions are expressions that have an operator in the middle (e.g. 1 + 1, true && false, 0b1010 | 0b0101)
        case NODE_EXPRESSION:
            if (end - start > 0) {
                // splitting the expression into blocks of parentheses and operators
                int p_values[] = OPERATOR_PRECEDENCE;
                int exit_loop = 0;
                for (int o = 0; o < (end - start)/2 && !exit_loop; o++) { // o is the offset, discarding the first and last token when parentheses are present
                    for (int p = 15; p > 0 && !exit_loop; p--) { // looping through the precedence values, meaning that the highest precedence is checked first
                        for (int i = end - o; i >= start + o; i--) { // looping backwards through the tokens
                            if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry & (BRACKET_ROUND | BRACKET_SQUARE)) {
                                i = getBlockReverse(tokens, i);
                                if (tokens[i].carry & BRACKET_SQUARE) exit_loop = 1; // exit the loops
                            }
                            if (tokens[i].type == TOKEN_OPERATOR && p_values[tokens[i].carry] == p) {
                                if (!(tokens[i-1].type == TOKEN_IDENTIFIER || tokens[i-1].type == TOKEN_STRING || tokens[i-1].type == TOKEN_NUMBER || full_code[tokens[i-1].start] == ')')) {
                                    exit_loop = 1; // exit the loops
                                    continue;
                                }
                                addToBody(&root.body, parse(full_code, tokens, start + o, i-1, NODE_EXPRESSION));
                                addToBody(&root.body, parse(full_code, tokens, i, i, NODE_OPERATOR));
                                addToBody(&root.body, parse(full_code, tokens, i+1, end - o, NODE_EXPRESSION));
                                return root;
                            }
                        }
                    }
                }
                // if no operators were found, check if the expression is valid
                for (int o = 0; o < (end - start + 2)/2; o++) {
                    if (tokens[start + o].type == TOKEN_PARENTHESIS && tokens[start + o].carry & BRACKET_SQUARE) {
                        addToBody(&root.body, parse(full_code, tokens, start + o, end - o, NODE_ARRAY_EXPRESSION));
                        return root;
                    }
                    if (getBlock(tokens, start + o) != end - o) {
                        if (tokens[start + o].type == TOKEN_OPERATOR) {
                            addToBody(&root.body, parse(full_code, tokens, start + o, end - o, NODE_UNARY_EXPRESSION));
                        }
                        else {
                            if (end - o - start + o > 0) printError(full_code, tokens[start + o].start, ERROR_INVALID_EXPRESSION);
                            addToBody(&root.body, parse(full_code, tokens, start + o, end - o, NODE_EXPRESSION));
                        }
                        return root;
                    }
                    
                    if (o == (end - start + 2)/2 - 1) {
                        addToBody(&root.body, parse(full_code, tokens, start + o, end - o, NODE_EXPRESSION));
                        return root;
                    }
                }
                printError(full_code, tokens[start].start, ERROR_INVALID_EXPRESSION);
            } else if (end - start == 0) {
                // if the expression is only one token, check if it's a literal or identifier
                if (tokens[start].type == TOKEN_IDENTIFIER) {
                    root.type = NODE_IDENTIFIER;
                } else if (tokens[start].type == TOKEN_NUMBER || tokens[start].type == TOKEN_STRING) {
                    root.type = NODE_LITERAL;
                } else {
                    printError(full_code, tokens[start].start, ERROR_EXPECTED_IDENTIFIER);
                }
                break;
            }
            printError(full_code, tokens[start].start, ERROR_EXPECTED_EXPRESSION);
            break;
        // Unary expressions are expressions that start with an operator (e.g. -1, !true, ~0b1010)
        case NODE_UNARY_EXPRESSION:
            if (tokens[start].type != TOKEN_OPERATOR || (tokens[start].carry != OPERATOR_SUBTRACT && tokens[start].carry != OPERATOR_NOT_BITWISE && tokens[start].carry != OPERATOR_NOT)) {
                printError(full_code, tokens[start].start, ERROR_OPERATOR_NOT_UNARY);
            }
            addToBody(&root.body, parse(full_code, tokens, start, start, NODE_OPERATOR));
            addToBody(&root.body, parse(full_code, tokens, start+1, end, NODE_EXPRESSION));
            break;
        case NODE_IDENTIFIER:
        case NODE_LITERAL:
        case NODE_OPERATOR:
        
            // these are base nodes and therefore don't need to be parsed
            break;

    }
    return root;
}

#endif
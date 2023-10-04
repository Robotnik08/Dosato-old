/**
 * @file parser.h
 * @brief The parser is used to parse a list of tokens into an AST
 * @version 0.0.3
 * @date 05-10-2023
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"
#include "log.h"
#include "str_tools.h"
/**
 * @brief The Node Type is used to represent a node in the AST
*/
typedef struct Node Node;

struct Node {
    int start, end;
    NodeType type;
    Node* body;
};

/**
 * @brief Create a null terminated node
 * @return The null terminated node
*/
Node createNullTerminatedNode();

/**
 * @brief Add a node to the body of a node
 * @param body The body to add the node to
 * @param node The node to add to the body
*/
void addToBody (Node** body, Node node);

/**
 * @brief Get the full line starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of the last token of the line
*/
int getFullLine (Token* tokens, const int start);

/**
 * @brief Get the block starting at a token
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of thelast token of the block
*/
int getBlock (Token* tokens, const int start);

/**
 * @brief Get the block starting at a token, but reads backwards
 * @param tokens The list of tokens
 * @param start The index of the token to start at
 * @return The index of the first token of the block
*/
int getBlockReverse (Token* tokens, const int start);

/**
 * @brief Parse a list of tokens into an AST
 * @param tokens The list of tokens to parse
 * @param start The index of the first token to parse
 * @param end The index of the last token to parse
 * @return The resulting AST from the list of tokens (don't forget to destroy it)
*/
Node parse (const char* code, Token* tokens, const int start, const int end, const NodeType type);

/**
 * @brief Get the amount of nodes in a node's body 
 * @param nodes The node's body
 * @return The amount of nodes in the body (excluding the null terminator)
*/
int getBodyLength (const Node* nodes);

/**
 * @brief Destroy a node and all of it's children, freeing the memory
 * @param node The node to destroy
*/
void destroyNode (Node* node);

/**
 * @brief Get the string version of a node type
 * @param type The node type
 * @return The string version of the node type
*/
char* getNodeTypeString(NodeType type);

/**
 * @brief Get the string of the full code from a node, start of first token and end of last token
 * @param full_code The full code
 * @param tokens The list of tokens
 * @param node The node to get the string version of
 * @param string The string version of the node
 * @return The string version of the node (don't forget to free it)
*/
void getStringFromNode (const char* full_code, const Token* tokens, const Node* node, char** string);

/**
 * @brief Print a node
 * @param node The node to print
 * @param depth The depth of the node (this is used for indentation, root should be 1)
 * @param isRoot Whether or not the node is the root node (boolean)
*/
void printNode (const char* full_code, const Token* tokens, const Node* node, const int depth, int isRoot);




Node createNullTerminatedNode () {
    Node node;
    node.start = 0;
    node.end = 0;
    node.type = NODE_END;
    node.body = NULL;
    return node;
}

void addToBody (Node** body, const Node node) {
    int length = getBodyLength(*body);
    *body = realloc(*body, sizeof(Node) * (length + 2));
    (*body)[length] = node;

    (*body)[length + 1] = createNullTerminatedNode();
}

int getFullLine (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    for (int i = start; i < length; i++) {
        if (tokens[i].type == TOKEN_PARENTHESIS) {
            i = getBlock(tokens, i);
        }
        if (tokens[i].type == TOKEN_SEPARATOR) {
            return i;
        }
    }
    return length-1;
}

int getBlock (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    int targetCarry = tokens[start].carry;
    for (int i = start+1; i < length; i++) {
        if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry == targetCarry) {
            return i;
        }
    }
    return length-1;
}
int getBlockReverse (Token* tokens, const int start) {
    int length = getTokenAmount(tokens);
    int targetCarry = tokens[start].carry;
    for (int i = start-1; i >= 0; i--) {
        if (tokens[i].type == TOKEN_PARENTHESIS && tokens[i].carry == targetCarry) {
            return i;
        }
    }
    return 0;
}

int getBodyLength (const Node* nodes) {
    if (nodes == NULL) return 0;
    int i = 0;
    while (nodes[i].type != NODE_END) {
        i++;
    }
    return i;
}

void destroyNode (Node* node) {
    if (node->body != NULL) {
        for (int i = 0; i < getBodyLength(node->body); i++) {
            destroyNode(&node->body[i]);
        }
        free(node->body);
    }
}

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
        
        // if the node is a function call, check for identifiers and extensions
        case NODE_FUNCTION_CALL:
            int got_identifier = 0;
            for (int i = start; i < end; i++) {
                if (tokens[i].type == TOKEN_IDENTIFIER) {
                    if (got_identifier) printError(full_code, tokens[i].start, ERROR_EXPECTED_EXTENSION);
                    int args_end = getBlock(tokens, i+1);
                    addToBody(&root.body, parse(full_code, tokens, i, args_end, NODE_FUNCTION_IDENTIFIER));
                    i = args_end;
                    got_identifier = 1;
                } else if (tokens[i].type == TOKEN_EXT) {
                    int t_end = end;
                    const ExtensionKeywordType types[] = EXTENSION_ACCEPTS;
                    Node ext_root;
                    switch (types[tokens[i].carry])
                    {
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
                        case NEEDS_IDENTIFIER:
                            if (tokens[i+1].type != TOKEN_IDENTIFIER) {
                                printError(full_code, tokens[i+1].start, ERROR_EXPECTED_IDENTIFIER);
                            }
                            t_end = i+1;
                            ext_root = parse(full_code, tokens, i, t_end, NODE_WHEN + tokens[i].carry);
                            addToBody(&ext_root.body, parse(full_code, tokens, i+1, t_end, NODE_IDENTIFIER));
                            break;
                        case NEEDS_EXPRESSION:
                            if (tokens[i+1].type == TOKEN_PARENTHESIS && tokens[i+1].carry & BRACKET_ROUND) {
                                t_end = getBlock(tokens, i+1);
                                ext_root = parse(full_code, tokens, i, t_end, NODE_WHEN + tokens[i].carry);
                                addToBody(&ext_root.body, parse(full_code, tokens, i+1, t_end, NODE_EXPRESSION));
                                break;
                            }
                            printError(full_code, tokens[i+1].start, ERROR_EXPECTED_EXPRESSION);
                    }
                    addToBody(&root.body, ext_root);
                    i = t_end;
                } else {
                    printError(full_code, tokens[i].start, ERROR_EXPECTED_EXTENSION);
                }
            }
            break;
        // MAKING a variable or function
        case NODE_MAKE_VAR:
            if (tokens[start].type != TOKEN_VAR_TYPE) {
                printError(full_code, tokens[start].start, ERROR_EXPECTED_TYPE);
            }
            addToBody(&root.body, parse(full_code, tokens, start, start, NODE_TYPE_IDENTIFIER));
            if (tokens[start + 1].type != TOKEN_IDENTIFIER) {
                printError(full_code, tokens[start + 1].start, ERROR_EXPECTED_IDENTIFIER);
            }
            if (tokens[start].carry == TYPE_FUNC) {
                root.type = NODE_FUNCTION_DECLARATION;
                if (tokens[start + 2].type != TOKEN_PARENTHESIS || !(tokens[start + 2].carry & BRACKET_ROUND)) {
                    printError(full_code, tokens[start + 2].start, ERROR_EXPECTED_ARGUMENTS);
                }
                int args_end = getBlock(tokens, start + 2);
                addToBody(&root.body, parse(full_code, tokens, start + 2, args_end, NODE_ARGUMENTS));
                if (tokens[args_end + 1].type != TOKEN_PARENTHESIS || !(tokens[args_end + 1].carry & BRACKET_CURLY)) {
                    printError(full_code, tokens[args_end + 1].start, ERROR_EXPECTED_BLOCK);
                }
                int block_end = getBlock(tokens, args_end + 1);
                if (block_end + 1 != end) {
                    printError(full_code, tokens[block_end + 1].start, ERROR_EXPECTED_SEPERATOR);
                }
                if (block_end - 2 - args_end <= 0) {
                    printError(full_code, tokens[args_end + 1].start, ERROR_EMPTY_BLOCK);
                } 
                addToBody(&root.body, parse(full_code, tokens, args_end + 2, block_end-1, NODE_BLOCK));
                break;
            }
            if (tokens[start + 2].type == TOKEN_OPERATOR && tokens[start + 2].carry == OPERATOR_ASSIGN) {
                addToBody(&root.body, parse(full_code, tokens, start + 1, start + 1, NODE_IDENTIFIER));
                addToBody(&root.body, parse(full_code, tokens, start + 3, end-1, NODE_EXPRESSION));
            } else {
                printError(full_code, tokens[start + 2].start, ERROR_EXPECTED_ASSIGN_OPERATOR);
            }
            break;
        // if the node is a function identifier, check for arguments
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
        // if the node is a function call, check for arguments
        case NODE_ARGUMENTS:
            int arg_start = start + 1;
            for (int i = start + 1; i < end - 1; i++) {
                if (tokens[i].type == TOKEN_OPERATOR && tokens[i].carry == OPERATOR_COMMA) {
                    addToBody(&root.body, parse(full_code, tokens, arg_start, i-1, NODE_ARGUMENT));
                    arg_start = i + 1;
                }
            }
            if (arg_start > end-1) printError(full_code, tokens[arg_start].start, ERROR_EXPECTED_ARGUMENT);
            addToBody(&root.body, parse(full_code, tokens, arg_start, end-1, NODE_ARGUMENT));
            break;
        // if the node is an argument, check for identifiers, literals and expressions
        case NODE_ARGUMENT:
            if (start - end == 0) {
                if (tokens[start].type == TOKEN_IDENTIFIER) {
                    addToBody(&root.body, parse(full_code, tokens, start, end, NODE_IDENTIFIER));
                } else if (tokens[start].type == TOKEN_NUMBER || tokens[start].type == TOKEN_STRING) {
                    addToBody(&root.body, parse(full_code, tokens, start, end, NODE_LITERAL));
                } else {
                    printError(full_code, tokens[start].start, ERROR_EXPECTED_IDENTIFIER);
                }
            }
            else {
                addToBody(&root.body, parse(full_code, tokens, start, end, NODE_EXPRESSION));
            }
            break;
        // if the node is an expression, check for operators
        case NODE_EXPRESSION:
            // work in progress
            if (end - start > 0) {
                int p_values[] = OPERATOR_PRECEDENCE;
                for (int o = 0; o < end-start/2; o++) { // o is the offset, discarding the first and last token when parentheses are present
                    for (int p = 15; p > 0; p--) { // looping through the precedence values, meaning that the highest precedence is checked first
                        for (int i = end - o; i >= start + o; i--) { // looping backwards through the tokens
                            if (tokens[i].type == TOKEN_PARENTHESIS) {
                                i = getBlockReverse(tokens, i);
                            }
                            if (tokens[i].type == TOKEN_OPERATOR && p_values[tokens[i].carry] == p) {
                                addToBody(&root.body, parse(full_code, tokens, start + o, i-1, NODE_EXPRESSION));
                                addToBody(&root.body, parse(full_code, tokens, i, i, NODE_OPERATOR));
                                addToBody(&root.body, parse(full_code, tokens, i+1, end - o, NODE_EXPRESSION));
                                return root;
                            }
                        }
                    }
                }
            } else {
                if (tokens[start].type == TOKEN_IDENTIFIER) {
                    root.type = NODE_IDENTIFIER;
                } else if (tokens[start].type == TOKEN_NUMBER || tokens[start].type == TOKEN_STRING) {
                    root.type = NODE_LITERAL;
                } else {
                    // printError(full_code, tokens[start].start, ERROR_EXPECTED_IDENTIFIER);
                }
            }
            break;
        
        case NODE_IDENTIFIER:
        case NODE_LITERAL:
        case NODE_OPERATOR:
            // these are base nodes and therefore don't need to be parsed
            break;

    }
    return root;
}


/**
 * debug functions for printing the AST
*/

char* getNodeTypeString(NodeType type) {
    char* nodeTypeString = NULL;

    switch (type) {
        case NODE_NULL:
            nodeTypeString = "NODE_NULL";
            break;
        case NODE_PROGRAM:
            nodeTypeString = "program";
            break;
        case NODE_FUNCTION_CALL:
            nodeTypeString = "function call";
            break;
        case NODE_MAKE_VAR:
            nodeTypeString = "make_var";
            break;
        case NODE_SET_VAR:
            nodeTypeString = "set_var";
            break;
        case NODE_FUNCTION_DECLARATION:
            nodeTypeString = "function declaration";
            break;
        case NODE_EXPRESSION:
            nodeTypeString = "expression";
            break;
        case NODE_STATEMENT:
            nodeTypeString = "statement";
            break;
        case NODE_LITERAL:
            nodeTypeString = "literal";
            break;
        case NODE_IDENTIFIER:   
            nodeTypeString = "identifier";
            break;
        case NODE_OPERATOR: 
            nodeTypeString = "operator";
            break;
        case NODE_BLOCK:
            nodeTypeString = "block";
            break;
        case NODE_FUNCTION_IDENTIFIER:
            nodeTypeString = "function identifier";
            break;
        case NODE_ARGUMENTS:    
            nodeTypeString = "arguments";
            break;
        case NODE_ARGUMENT:
            nodeTypeString = "argument";
            break;
        case NODE_TYPE_IDENTIFIER:
            nodeTypeString = "type_identifier";
            break;
        case NODE_WHEN:
            nodeTypeString = "when";
            break;
        case NODE_WHILE:
            nodeTypeString = "while";
            break;
        case NODE_ELSE:
            nodeTypeString = "else";
            break;
        case NODE_CATCH:    
            nodeTypeString = "catch";
            break;
        case NODE_INTO:
            nodeTypeString = "into";
            break;
        case NODE_THEN:
            nodeTypeString = "then";
            break;
        case NODE_END:
            nodeTypeString = "NODE_END";
            break;
        default:
            nodeTypeString = "Unknown"; // Return a default value for invalid types
            break;
    }

    return nodeTypeString;
}
 
void getStringFromNode (const char* full_code, const Token* tokens, const Node* node, char** string) {
    if (node->start > node->end) {
        *string = "Error: start > end";
        return;
    }
    *string = (char*)malloc(sizeof(char) * (tokens[node->end].end - tokens[node->start].start + 1));

    int stringIndex = 0;
    for (int i = tokens[node->start].start; i <= tokens[node->end].end; i++) {
        (*string)[stringIndex++] = full_code[i] == '\n' ? ' ' : full_code[i];
    }
    (*string)[stringIndex] = '\0';
}

void printNode (const char* full_code, const Token* tokens, const Node* node, int depth, int isRoot) {
    if (isRoot) {
        printf("{\n");
        printf("  \"root\": ");
    } else {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
    }
    printf("{\n");

    // Print the node position
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"start\": %i,\n", node->start);

    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"end\": %i,\n", node->end);

    // Print the node type
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    printf("\"type\": \"%s\",\n", getNodeTypeString(node->type));

    
    for (int i = 0; i < depth+1; i++) {
        printf("  ");
    }
    char *stringFromNode;
    getStringFromNode(full_code, tokens, node, &stringFromNode);
    printf("\"text\": \"%s\"", stringFromNode);
    free(stringFromNode);

    if (node->body != NULL) {
        printf(",\n");
        for (int i = 0; i < depth +1; i++) {
            printf("  ");
        }
        printf("\"body\": [\n");
        for (int i = 0; i < getBodyLength(node->body); i++) {
            printNode(full_code, tokens, &node->body[i], depth + 2, 0);
            if (i != getBodyLength(node->body) - 1) {
                printf(",\n");
            }
        }
        printf("\n");
        for (int i = 0; i < depth+1; i++) {
            printf("  ");
        }
        printf("]\n");
    } else {
        printf("\n");
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("}");
    if (isRoot) {
        printf("\n}");
    }
}


#endif
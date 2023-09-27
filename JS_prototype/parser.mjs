// brainstorming for a parser

export class ASTNode {
    constructor(type, value) {
        this.type = type;
        this.value = value;
        this.children = [];
    }

    addChild(node) {
        console.log(12);
        this.children.push(node);
    }
}

export class Parser {
    constructor(tokens) {
        this.tokens = tokens;
        this.currentTokenIndex = 0;
    }

    parse() {
        const ast = this.parseProgram();
        return ast;
    }

    parseProgram() {
        const programNode = new ASTNode('Program', '');

        while (this.currentTokenIndex < this.tokens.length) {
            const statement = this.parseStatement();
            programNode.addChild(statement);
        }

        return programNode;
    }

    parseStatement() {
        const token = this.getCurrentToken();

        // Parse variable declaration
        if (token.type === 'var_type') {
            const variableDeclarationNode = new ASTNode('VariableDeclaration', token.value);
            this.consumeToken(); // Consume the variable type token

            const identifierToken = this.getCurrentToken();
            if (identifierToken.type === 'general') {
                variableDeclarationNode.addChild(new ASTNode('Identifier', identifierToken.value));
                this.consumeToken();
            } else {
                throw new Error('Expected an identifier');
            }

            return variableDeclarationNode;
        }

        // Parse assignment statement
        if (token.type === 'general' && this.peekNextToken()?.value === '=') {
            const assignmentNode = new ASTNode('Assignment', token.value);
            this.consumeToken(); // Consume the variable identifier

            this.consumeToken(); // Consume the '=' token

            const expressionNode = this.parseExpression();
            assignmentNode.addChild(expressionNode);

            return assignmentNode;
        }
    }

    parseExpression() {

        const token = this.getCurrentToken();
        if (token.type === 'general') {
            this.consumeToken();
            return new ASTNode('Identifier', token.value);
        }

        throw new Error('Invalid expression');
    }

    getCurrentToken() {
        return this.tokens[this.currentTokenIndex];
    }

    peekNextToken() {
        return this.tokens[this.currentTokenIndex + 1];
    }

    consumeToken() {
        this.currentTokenIndex++;
    }
}
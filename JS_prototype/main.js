import { getTokens } from './lexer.mjs';
import { Parser } from './parser.mjs';

// JavaScript code to handle the interaction
const inputText = document.getElementById('inputText');
const outputDiv = document.getElementById('outputDiv');

let tokens = [];
// Add an event listener to update the output when text is entered
inputText.addEventListener('input', () => {
    tokens = getTokens(inputText.value);
    displayTokens(inputText.value, tokens);
    // const parser = new Parser(tokens);
    // const ast = parser.parse();
    // console.log(ast);
});
function displayTokens(inputText, t) {
    outputDiv.textContent = '';
    const code = document.createElement('code');

    for (let i = 0; i < inputText.length; i++) {
        let tokenFound = false;

        for (let token of t) {
            if (i == token.start) {
                const span = document.createElement('span');
                span.className = token.type;
                span.textContent = inputText.substring(token.start, token.end + 1);
                i = token.end;
                code.appendChild(span);
                tokenFound = true;
                break;
            }
        }

        if (!tokenFound) {
            const emptySpan = document.createElement('span');
            emptySpan.className = 'empty';
            emptySpan.textContent = inputText[i];
            code.appendChild(emptySpan);
        }
    }
    outputDiv.appendChild(code);
}
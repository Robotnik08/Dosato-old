export function getTokens(text) {
    let tokens = [];
    let start = 0;
    let end = 0;
    let quotationtype = '';
    let escapeCount = 0;
    let inComment = false;

    // get comment and string tokens
    for (let i = 0; i < text.length; i++) {
        if (quotationtype === '') {
            if ((text[i] === '"' || text[i] === "'") && escapeCount % 2 === 0) {
                quotationtype = text[i];
                start = i;
                continue;
            }
            if (text[i] === '/' && text[i + 1] === '/' && !inComment) {
                let foundEnd = false;
                start = i;
                for (let j = i; j < text.length; j++) {
                    if (text[j] === '\n') {
                        end = j - 1;
                        tokens.push(new Token('comment', start, end));
                        start = end + 1;
                        i = j;
                        foundEnd = true;
                        break;
                    }
                }
                if (!foundEnd) {
                    end = text.length - 1;
                    tokens.push(new Token('comment', start, end));
                    start = end + 1;
                    i = text.length;
                }
            }
        } else {
            if (text[i] === quotationtype && escapeCount % 2 === 0) {
                end = i;
                tokens.push(new Token('string', start, end));
                start = end + 1;
                quotationtype = '';
            }
        }

        // Check for escape character
        if (text[i] === '\\') {
            escapeCount++;
        } else {
            escapeCount = 0;
        }
    }

    // get master tokens
    const mastertokens = ["DO", "MAKE", "SET"];
    for (let i = 0; i < text.length; i++) {
        for (let t of tokens) {
            if (i == t.start && (t.type == 'string' || t.type == 'comment')) {
                i = t.end + 1;
            }
        }
        for (let j of mastertokens) {
            if (getWord(text, i) == j) {
                tokens.push(new Token('master', i, i + j.length - 1));
                i += j.length - 1;
                break;
            }
        }
    }

    // get var_type tokens
    const var_typetokens = ["INT", "BOOL", "STRING", "FLOAT", "DOUBLE", "CHAR", "SHORT", "LONG", "BYTE", "VOID", "ARRAY", "FUNC"];
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment')) {
                i = tokens[t].end + 1;
            }
        }
        for (let j in var_typetokens) {
            if (getWord(text, i) == var_typetokens[j]) {
                tokens.push(new Token('var_type', i, i + var_typetokens[j].length - 1));
                i += var_typetokens[j].length - 1;
                break;
            }
        }
    }

    // get extension tokens
    const extension_tokens = [
        "WHEN", "WHILE", "ELSE", "ELSEWHEN", "CATCH", "STORE", "THEN" 
    ];
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment')) {
                i = tokens[t].end + 1;
            }
        }
        for (let j in extension_tokens) {
            if (getWord(text, i) == extension_tokens[j]) {
                tokens.push(new Token('extension', i, i + extension_tokens[j].length - 1));
                i += extension_tokens[j].length - 1;
                break;
            }
        }
    }

    // get bracket tokens
    const brackettokens = ["()", "{}", "[]"];
    let bracketTier = 0;
    let bracketTypeHiarcy = [];
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment')) {
                i = tokens[t].end + 1;
            }
        }
        // check for opening brackets
        for (let j = 0; j < brackettokens.length; j++) {
            if (text[i] == brackettokens[j][0]) {
                bracketTier++;
                tokens.push(new Token('bracket' + ((bracketTier - 1) % 3), i, i, bracketTier));
                bracketTypeHiarcy.push(j);
                break;
            }
        }
        // check for closing brackets
        for (let j = 0; j < brackettokens.length; j++) {
            if (text[i] == brackettokens[j][1]) {
                tokens.push(new Token('bracket' + ((bracketTier - 1) % 3), i, i, bracketTier));
                if (bracketTypeHiarcy[bracketTypeHiarcy.length - 1] == j) {
                    bracketTypeHiarcy.pop();
                    bracketTier--;
                }
                break;
            }
        }
    }

    // get separator tokens
    const separatortokens = [";"];
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment')) {
                i = tokens[t].end + 1;
            }
        }
        for (let j in separatortokens) {
            if (text[i] == separatortokens[j]) {
                tokens.push(new Token('separator', i, i + separatortokens[j].length - 1));
                i += separatortokens[j].length - 1;
                break;
            }
        }
    }

    // getNumber tokens
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment')) {
                i = tokens[t].end + 1;
            }
        }
        if (isNumeric(text[i])) {
            if (text[i] == '.') {
                if (!isNumeric(text[i + 1])) continue;
            }
            let start = i;
            let end = i;
            let foundDecimal = false;

            let invalid = false;
            for (let j = i; j < text.length; j++) {
                if (!isNumeric(text[j-1]) && !isEmpty(text[j-1])) {
                    invalid = true;
                    for (let k = j; k < text.length && isNumeric(text[k]); k++) {
                        end = k;
                    }
                    break;
                }
                if (isNumeric(text[j])) {
                    end = j;
                    if (text[j] == '.' && foundDecimal) break;
                } else if (text[j] == '.' && !foundDecimal) {
                    end = j;
                    foundDecimal = true;
                } else {
                    break;
                }
            }
            if (!invalid) tokens.push(new Token('number', start, end));
            i = end;
        }
    }

    // get operator tokens
    const operatortokens = ["+", "-", "*", "/", "%", "=", ">", "<", "!", "&", "|", "^", "~", "?", ":", ".", ",", "#",
                            "+=", "-=", "*=", "/=", "%=", "++", "--", "==", "!=", ">=", "<=", "&&", "||", "<<", ">>",
    ];
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start && (tokens[t].type == 'string' || tokens[t].type == 'comment' || tokens[t].type == 'number')) {
                i = tokens[t].end + 1;
            }
        }
        let foundBig = false;
        for (let j in operatortokens) {
            if (text[i] + text[i+1] == operatortokens[j]) {
                tokens.push(new Token('operator', i, i + operatortokens[j].length - 1));
                i += operatortokens[j].length-1;
                foundBig = true;
                break;
            }
        }
        if (foundBig) continue;
        for (let j in operatortokens) {
            if (text[i] == operatortokens[j]) {
                tokens.push(new Token('operator', i, i + operatortokens[j].length - 1));
                i += operatortokens[j].length-1;
                break;
            }
        }
    }


    // get general tokens (variables, functions, etc.)
    for (let i = 0; i < text.length; i++) {
        for (let t in tokens) {
            if (i == tokens[t].start) {
                i = tokens[t].end + 1;
            }
        }
        if (isAlphaNumeric(text[i])) {
            let word = getWord(text, i);
            if (word.length > 0) {
                tokens.push(new Token('general', i, i + word.length - 1));
                i += word.length - 1;
            }
        }
    }

    sortTokens(tokens);
    return tokens;
}

function sortTokens (tokens) {
    tokens.sort((a, b) => {
        if (a.start < b.start) return -1;
        if (a.start > b.start) return 1;
        return 0;
    });
}
export function printTokens (tokens) {
    let c = 0;
    for (let t of tokens) {
        console.log(`${c}. ${t.toString()}`);
        c++;
    }
}

function getWord (text, start) {
    let word = '';
    for (let i = start; i < text.length; i++) {
        if (!isAlphaNumeric(text[i])) break;
        word += text[i];
    }
    return word;
}

function isAlphaNumeric (char) {
    if (!char) return false;
    return char.match(/^[a-z0-9_]+$/i);
}
function isEmpty (char) {
    if (!char) return false;
    return char.match(/^[\s]+$/i);
}
function isNumeric (char) {
    if (!char) return false;
    return char.match(/^[0-9.]+$/i);
}

class Token {
    constructor(type, start, end, meta = 0) {
        this.type = type;
        this.start = start;
        this.end = end;
        this.meta = meta;
    }

    toString () {
        return `{ type: ${this.type}, start: ${this.start}, end: ${this.end}, meta: ${this.meta}}`;
    }
}
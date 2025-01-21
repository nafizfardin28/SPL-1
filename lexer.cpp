#include "lexer.h"
#include <cctype>
#include <unordered_set>

bool isDelimiter(char c) {
    static const std::unordered_set<char> delimiters = {
        '(', ')', '{', '}', '[', ']', ';', ',', '.', ':', '?'
    };
    return delimiters.find(c) != delimiters.end();
}

bool isOperator(char c) {
    static const std::unordered_set<char> operators = {
        '+', '-', '*', '/', '%', '=', '<', '>', '!', '&', '|', '^', '~'
    };
    return operators.find(c) != operators.end();
}

bool isKeyword(const std::string& str) {
    static const std::unordered_set<std::string> keywords = {
        "main","auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    return keywords.find(str) != keywords.end();
}

TokenCount tokenizeAndCount(const std::string& code) {
    TokenCount count = {0, 0, 0, 0, 0, 0};
    std::string currentToken;
    bool inString = false;
    bool inChar = false;
    bool inComment = false;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];
        char nextC = (i + 1 < code.length()) ? code[i + 1] : '\0';

        if (inComment) {
            if (c == '*' && nextC == '/') {
                inComment = false;
                i++;
            }
            continue;
        }
        if (c == '/' && nextC == '*') {
            inComment = true;
            i++;
            continue;
        }

        if (c == '/' && nextC == '/') {
            while (i < code.length() && code[i] != '\n') i++;
            continue;
        }

        if (inString) {
            if (c == '"' && code[i - 1] != '\\') inString = false;
            continue;
        }
        if (c == '"') {
            inString = true;
            continue;
        }

        
        if (inChar) {
            if (c == '\'' && code[i - 1] != '\\') inChar = false;
            continue;
        }
        if (c == '\'') {
            inChar = true;
            continue;
        }

        if (std::isspace(c) || isDelimiter(c) || isOperator(c)) {
            if (!currentToken.empty()) {
                if (isKeyword(currentToken)) {
                    count.keyWordCount++;
                } else if (std::isalpha(currentToken[0]) || currentToken[0] == '_') {
                    count.identifierCount++;
                } else if (std::isdigit(currentToken[0])) {
                    count.numberCount++;
                } else {
                    count.unknownCount++;
                }
                currentToken.clear();
            }

            if (isDelimiter(c)) {
                count.delimiterCount++;
            } else if (isOperator(c)) {
                count.operatorCount++;
            }
            continue;
        }
        currentToken += c;
    }

    if (!currentToken.empty()) {
        if (isKeyword(currentToken)) {
            count.keyWordCount++;
        } else if (std::isalpha(currentToken[0]) || currentToken[0] == '_') {
            count.identifierCount++;
        } else if (std::isdigit(currentToken[0])) {
            count.numberCount++;
        } else {
            count.unknownCount++;
        }
    }

    return count;
}

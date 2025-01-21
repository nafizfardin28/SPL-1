#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <string>

struct TokenCount
{
    int keyWordCount;
    int identifierCount;
    int operatorCount;
    int numberCount;
    int delimiterCount;
    int unknownCount;
};

TokenCount tokenizeAndCount(const std::string &code);

#endif // LEXER_H
#include "cyclomatic.h"
#include "removeComment.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include<set>
#include<unordered_map>
using namespace std;

int countDecisionPoints(const vector<string> &codeLines)
{
    int decisionCount = 0;

    regex decisionRegex("\\b(if|else if|while|for|switch|case|catch|&&|\\|\\|)\\b");

    for (const string &line : codeLines)
    {
        auto wordsBegin = sregex_iterator(line.begin(), line.end(), decisionRegex);
        auto wordsEnd = sregex_iterator();
        decisionCount += distance(wordsBegin, wordsEnd);
    }
    return decisionCount;
}

Cyclomatic calculateCyclomaticComplexity(const vector<string> &codeLines)
{
    string code;
    for (const auto& line : codeLines) {
        code += line + "\n";
    }
    code = removeComments(code);

    vector<string> cleanedCodeLines;
    istringstream stream(code);
    string line;
    while (getline(stream, line)) {
        cleanedCodeLines.push_back(line);
    }

    Cyclomatic metrics;
    metrics.cyclomaticComplexity = countDecisionPoints(cleanedCodeLines) + 1;
    
    return metrics;
}

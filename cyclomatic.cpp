#include "cyclomatic.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

int countDecisionPoints(const vector<string> &codeLines)
{
    int decisionCount = 0;

    regex decisionRegex("\\b(if|else if|while|for|switch)\\b");

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
    Cyclomatic metrics;
    metrics.cyclomaticComplexity = countDecisionPoints(codeLines) + 1;
    
    return metrics;
}

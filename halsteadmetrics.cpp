#include "halstead.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <cmath>

using namespace std;
bool isaOperator(char c) {
    string operators = "+-*/%=&|^~<>!";
    return operators.find(c) != string::npos;
}

bool isSeparator(char c) {
    string separators = " \t\n,;(){}[]";
    return separators.find(c) != string::npos;
}

HalsteadMetrics calculateHalsteadMetrics(const string& code) {
    unordered_set<string> uniqueOperators;
    unordered_set<string> uniqueOperands;
    unordered_map<string, int> operatorCount;
    unordered_map<string, int> operandCount;

    string token;
    bool isOperand = false;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

        if (isaOperator(c)) {
            if (!token.empty() && isOperand) {
                uniqueOperands.insert(token);
                operandCount[token]++;
                token.clear();
            }
            token += c;
            uniqueOperators.insert(token);
            operatorCount[token]++;
            token.clear();
            isOperand = false;
        } 
        else if (isSeparator(c)) {
            if (!token.empty() && isOperand) {
                uniqueOperands.insert(token);
                operandCount[token]++;
                token.clear();
            }
            isOperand = false;
        }
        else {
            token += c;
            isOperand = true;
        }
    }
    HalsteadMetrics halsteadMetrics;
    halsteadMetrics.n1 = uniqueOperators.size();
    halsteadMetrics.n2 = uniqueOperands.size();
    int N1 = 0;
    int N2 = 0;

    for (const auto& op : operatorCount) {
        N1 += op.second;
    }

    for (const auto& op : operandCount) {
        N2 += op.second;
    }
    halsteadMetrics.N1 = N1;
    halsteadMetrics.N2 = N2;
    halsteadMetrics.vocabulary = halsteadMetrics.n1 + halsteadMetrics.n2;
    halsteadMetrics.length = halsteadMetrics.N1 + halsteadMetrics.N2;
    halsteadMetrics.calculatedLength = halsteadMetrics.n1 * log2(halsteadMetrics.n1) + halsteadMetrics.n2 * log2(halsteadMetrics.n2);
    halsteadMetrics.volume = halsteadMetrics.length * log2(halsteadMetrics.vocabulary);
    halsteadMetrics.difficulty = (halsteadMetrics.n1 / 2) * (halsteadMetrics.N2 / halsteadMetrics.n2);
    halsteadMetrics.effort = halsteadMetrics.difficulty * halsteadMetrics.volume;
    halsteadMetrics.time = halsteadMetrics.effort / 18;
    halsteadMetrics.bugs = (halsteadMetrics.effort, (2/3)) / 3000;

    return halsteadMetrics;
    
}


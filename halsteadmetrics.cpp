#include "halstead.h"
#include "removeComment.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <regex>
#include <cmath>
using namespace std;
vector<string> operatorList = {
    ">>=", "<<=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "==", "!=", ">=", "<=",
    "&&", "||", "++", "--", "<<", ">>", "=", ">", "<", "+", "-", "*", "/", "%", "&", "|", "!", "^"};

void countOperators(const string &code, int &n1, int &N2)
{
    unordered_map<string, int> opCount;
    unordered_set<string> uniqueOps;
    string cleancode = removeComments(code);
    stringstream file(cleancode);
    string line;

    while (getline(file, line))
    {
        if (line.find("#include") == 0)
        {
            continue;
        }

        size_t i = 0;
        while (i < line.length())
        {
            if (isspace(line[i]))
            {
                i++;
                continue;
            }
            bool found = false;
            for (const string &op : operatorList)
            {
                if (line.substr(i, op.length()) == op)
                {
                    opCount[op]++;
                    uniqueOps.insert(op);
                    i += op.length();
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                i++;
            }
        }
    }

    for (const auto &entry : opCount)
    {
        N2 += entry.second;
    }
    n1 = uniqueOps.size();
}
void countOperands(const string &code,int& n2, int& N2)
{
    string cleancode = removeComments(code);
   
    regex operandRegex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");

    unordered_map<string, int> operandCount;
    sregex_iterator it(cleancode.begin(), cleancode.end(), operandRegex);
    sregex_iterator end;

    while (it != end)
    {
        operandCount[it->str()]++;
        ++it;
    }

    for (const auto &entry : operandCount)
    {
        N2 += entry.second;
    }
    n2 = operandCount.size();
}
HalsteadMetrics calculateHalsteadMetrics(const string &filename)
{
    HalsteadMetrics metrics = {0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0};
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error opening file: " << filename << endl;
        return metrics;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();
    file.close();
    countOperators(code, metrics.n1, metrics.N1);
    countOperands(code,metrics.n2,metrics.N2);
    metrics.vocabulary = metrics.n1 + metrics.n2;
    metrics.length = metrics.N1 + metrics.N2;
    metrics.calculatedLength = metrics.n1 * log2(metrics.n1) + metrics.n2 * log2(metrics.n2);
    metrics.volume = metrics.length * log2(metrics.vocabulary);
    metrics.difficulty = (metrics.n1 / 2) * (metrics.N2 / metrics.n2);
    metrics.effort = metrics.difficulty * metrics.volume;
    metrics.time = metrics.effort / 18;
    metrics.bugs = metrics.volume / 3000;
    return metrics;
}


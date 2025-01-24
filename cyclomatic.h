#ifndef CYCLOMATIC_H
#define CYCLOMATIC_H

using namespace std;

#include <vector>
#include <string>

struct Cyclomatic
{
    int cyclomaticComplexity;
    double cyclomaticDensity;
};
Cyclomatic calculateCyclomaticComplexity(const vector<string> &lines);
int isDecisionPoint(const string &lines);

#endif

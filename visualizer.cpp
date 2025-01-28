#include "metrics.h"
#include "lexer.h"
#include <iostream>
#include <iomanip>

using namespace std;

void visualizemetrics(const vector<FileMetrics>& allMetrics)
{
    cout << "CODE SIZE METRICS-" << endl << endl;
    cout << setw(20) << left << "FileName"
         << setw(15) << "TotalLines"
         << setw(15) << "CodeLines"
         << setw(15) << "BlankLines"
         << setw(20) << "TotalCommentLines"
         << setw(15) << "Functions"
         << setw(20) << "MultiCommentLines"
         << setw(15) << "Comment Ratio (%)";
    cout << endl;
    cout << string(137, '-') << endl;

    for (const auto& fileMetrics : allMetrics)
    {
        cout << setw(20) << left << fileMetrics.filename
             << setw(15) << fileMetrics.metrics.totalLines
             << setw(15) << (fileMetrics.metrics.codeLines - fileMetrics.metrics1.singleLineComments)
             << setw(15) << fileMetrics.metrics.blankLines
             << setw(20) << fileMetrics.metrics1.totalCommentLines
             << setw(15) << fileMetrics.metrics.functionCount
             << setw(20) << fileMetrics.metrics1.multiLineComments
             << setw(15) << fixed << setprecision(2)
             << ((fileMetrics.metrics.totalLines > 0)
                 ? (static_cast<double>(fileMetrics.metrics1.totalCommentLines) / fileMetrics.metrics.totalLines) * 100
                 : 0.0) ;
        cout << endl ;
    }
    cout << endl << "CYCLOMATIC COMPLEXITY -" << endl << endl;

    cout << setw(20) << left << "FileName"
         << setw(25) << "CyclomaticComplexity"
         << "CyclomaticDensity(%)" << endl;
    cout << string(65, '-') << endl;

    for(const auto& fileMetrics : allMetrics)
    {
        cout << setw(20) << left << fileMetrics.filename;
        cout <<"  " << setw(22) << left << fileMetrics.metrics3.cyclomaticComplexity
             <<" " << ((fileMetrics.metrics.totalLines > 0)
                 ? (static_cast<double>(fileMetrics.metrics3.cyclomaticComplexity) / fileMetrics.metrics.totalLines) * 100
                 : 0.0) ;
        cout << endl;
    }
    cout << endl << "TOKENS-" << endl << endl;
    //cout <<"--------" << endl;
    cout << setw(20) << left << "FileName"
         << setw(15) << "Keywords"
         << setw(15) << "Identifiers"
         << setw(15) << "Operators"
         << setw(15) << "Numbers"
         << setw(15) << "Delimiters"
         << setw(15) << "Unknowns";
    cout << endl;
    cout << string(103, '-') << endl;

    for (const auto& fileMetrics : allMetrics)
    {
        cout << setw(20) << left << fileMetrics.filename
             << setw(15) << fileMetrics.metrics2.keyWordCount
             << setw(15) << fileMetrics.metrics2.identifierCount
             << setw(15) << fileMetrics.metrics2.operatorCount
             << setw(15) << fileMetrics.metrics2.numberCount
             << setw(15) << fileMetrics.metrics2.delimiterCount
             << setw(15) << fileMetrics.metrics2.unknownCount;
        cout << endl;
    }
    cout << endl << "HALSTEAD METRICS-" << endl << endl;
    cout << setw(20) << left << "FileName"
         << setw(15) << "Length(N)"
         << setw(15) << "Vocabulary(n)   "
         << setw(15) << "Volume(V)"
         << setw(25) << "Calculated Length(N^)"
         << setw(15) << "Difficulty(D)"
         << setw(15) << "Effort(E)"
         << setw(15) << " Time(T)"
         << setw(15) <<  " Bugs(B)";
    cout << endl;
    cout << string(144, '-') << endl;

    for (const auto& fileMetrics : allMetrics)
    {
        cout << setw(20) << left << fileMetrics.filename
             << setw(15) << fileMetrics.metrics4.length
             << setw(17) << fileMetrics.metrics4.vocabulary
             << setw(17) << fileMetrics.metrics4.volume
             << setw(25) << fileMetrics.metrics4.calculatedLength
             << setw(15) << fileMetrics.metrics4.difficulty
             << setw(15) << fileMetrics.metrics4.effort
             << setw(15) << fileMetrics.metrics4.time
             << setw(15) << fileMetrics.metrics4.bugs;
        cout << endl;
    }
    cout << endl;
}

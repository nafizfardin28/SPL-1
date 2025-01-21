#include "metrics.h"
#include "lexer.h"
#include <iostream>
#include <iomanip>

using namespace std;

void visualizemetrics(const vector<FileMetrics>& allMetrics)
{
    cout << "CODE SIZE METRICS-" << endl << endl;
    cout << setw(12) << "FileName" << setw(15)
         << "TotalLines" << setw(12)
         << "CodeLines" << setw(12)
         << "BlankLines  " << setw(14)
         << "TotalCommentLines" << setw(13)
         << "Function  " << setw(13)
         << "MultiCommentLines" << setw(15)
         << "Comment Ratio";
    cout << endl;
    cout << string(125, '-') << endl;
    for (const auto& fileMetrics : allMetrics)
    {
        cout << setw(12) << fileMetrics.filename.substr(0, 17)
             << setw(10) << fileMetrics.metrics.totalLines
             << setw(13) << (fileMetrics.metrics.codeLines - fileMetrics.metrics1.singleLineComments)
             << setw(13) << fileMetrics.metrics.blankLines
             << setw(16) << fileMetrics.metrics1.totalCommentLines 
             << setw(15) << fileMetrics.metrics.functionCount
             << setw(13) << fileMetrics.metrics1.multiLineComments
             << setw(18) << fixed << setprecision(2) 
             << (double)(fileMetrics.metrics1.totalCommentLines) / (fileMetrics.metrics.totalLines) * 100 << "%";
        cout << endl;
    }
    
    cout << endl << "TOKEN METRICS-" << endl << endl;
    cout << setw(12) << "FileName" << setw(13)
         << "keyWords" << setw(14) 
         << "Identifiers" << setw(14)
         << "Operators" << setw(14)
         << "Numbers" << setw(14)
         << "Delimiters" << setw(12)
         << "Unknowns";
    cout << endl;
    cout << string(95, '-') << endl;
    for (const auto& fileMetrics : allMetrics)
    {
        cout << setw(12) << fileMetrics.filename.substr(0, 17)
             << setw(10) << fileMetrics.metrics2.keyWordCount
             << setw(13) << fileMetrics.metrics2.identifierCount
             << setw(13) << fileMetrics.metrics2.operatorCount
             << setw(13) << fileMetrics.metrics2.numberCount
             << setw(13) << fileMetrics.metrics2.delimiterCount
             << setw(13) << fileMetrics.metrics2.unknownCount;
        cout << endl;
    }
    cout << endl;
}

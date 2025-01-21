#include "metrics.h"
#include <iostream>
#include <iomanip>

using namespace std;

void visualizemetrics(const vector<FileMetrics> &allMetrics)
{
    cout << "CODE SIZE METRICS-" << endl << endl;
    cout << setw(12) << "FileName" << setw(15)
         << "TotalLines" << setw(14)
         << "CodeLines" << setw(14)
         << "BlankLines" << setw(16)
         << " TotalCommentLines " << setw(10)
         << "Function " << setw(15)
         << " MultiCommentLines" << setw(17)
         << "Comment Ratio";
    cout << endl;
    cout << string(125, '-') << endl;
    for (const auto &fileMetrics : allMetrics)
    {
        cout << setw(12) << fileMetrics.filename.substr(0, 17)
             << setw(10) << fileMetrics.metrics.totalLines
             << setw(13) << (fileMetrics.metrics.codeLines - fileMetrics.metrics1.singleLineComments)
             << setw(13) << fileMetrics.metrics.blankLines
             << setw(16) << fileMetrics.metrics1.totalCommentLines 
             << setw(15) << fileMetrics.metrics.functionCount
             << setw(13) << fileMetrics.metrics1.multiLineComments
             << setw(22) << fixed << setprecision(2) <<(double)(fileMetrics.metrics1.totalCommentLines)/(fileMetrics.metrics.totalLines);
        cout << endl;
    }
}
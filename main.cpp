#include "metrics.h"
#include "lexer.h"
#include <iostream>
#include <filesystem>
#include<fstream>
#include<string>
#include<numeric>
using namespace std;

int main(int argC, char* argV[])
{
    if(argC<2)
    {
        cout << "Usage : " << argV[0] << " <file1.cpp> [file2.cpp]... " << endl;
        return 1;
    }
    vector<FileMetrics> allMetrics;
    for(int i=1;i<argC;i++)
    {
        string filename = argV[i];
        if(!std::filesystem::exists(filename))
        {
            cerr << "File not found:" << filename << endl;
            continue;
        }
        vector<string> lines = readFileLines(filename);
        std::string code = accumulate(lines.begin(), lines.end(), std::string("\n"));
        TokenCount metrics2 = tokenizeAndCount(code);
        //TokenCount metrics2 = tokenizeAndCount(filename);
        sizeMetrics metrics = calculateMetrics(lines);
        CommentMetrics metrics1 = calculateComments(lines);
        allMetrics.push_back({filename,metrics,metrics1,metrics2});
    }
    visualizemetrics(allMetrics);
    return 0;

}

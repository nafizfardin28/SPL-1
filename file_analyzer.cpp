#include "metrics.h"
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
using namespace std;
//iiii
vector<string> readFileLines(const string &filename)
{
    vector<string> lines;
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        cerr << "Error: Unable to open file '" << filename << "'." << endl;
        return lines;
    }

    while (getline(file, line))
    {
        lines.push_back(line);
    }

    return lines;
}
CommentMetrics calculateComments(const vector<string> &lines)
{
    CommentMetrics metrics={0,0,0,0};
    bool inMultiLineComment = false;
    regex singleLineCommentRegex(R"(^\s*//|[^:"]//.*$)");
    regex multiLineCommentStartRegex(R"(/\*.*)");
    regex multiLineCommentEndRegex(R"(.*\*/)");
    for (const auto &line : lines)
    {
        metrics.totalLines++;
        if (inMultiLineComment)
        {
            metrics.totalCommentLines++;
            if (regex_search(line, multiLineCommentEndRegex))
            {
                inMultiLineComment = false;
            }
        }
        else
        {
            if (regex_search(line, singleLineCommentRegex))
            {
                metrics.singleLineComments++;
                metrics.totalCommentLines++;
            }
            if (regex_search(line, multiLineCommentStartRegex))
            {
                metrics.multiLineComments++;
                metrics.totalCommentLines++;
                if (!regex_search(line, multiLineCommentEndRegex))
                {
                    inMultiLineComment = true;
                }
            }
        }
    }
    return metrics;
}
bool isBlankLine(const string& line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char c) { return isspace(c); });
}
int countFunctions(const vector<string>& fileLines) {
    // Regular expression to match function definitions
    std::regex functionRegex(R"(\b(?:void|int|float|double|char|bool|long|short|unsigned|signed|auto|const|volatile|static|virtual|class|struct|template|inline|explicit|constexpr)\s+\w+\s*\([^)]*\)\s*\{)");
    int functionCount = 0;

    // Iterate through the lines of the file
    for (const auto& line : fileLines) {
        if (regex_search(line, functionRegex)) {
            functionCount++;
        }
    }

    return functionCount;
}
sizeMetrics calculateMetrics(const vector<string> &lines)
{
    sizeMetrics metrics = {0, 0, 0, 0};
    // CommentMetrics metrics1 = {0,0,0};
    bool inMultiLineComment = false;
    for (const auto &line : lines)
    {
        metrics.totalLines++;
        string trimmedLine = line;
        //trimmedLine.erase(0, trimmedLine.find_first_not_of("\t"));

        if(isBlankLine(line))
            metrics.blankLines++;
    }
    metrics.functionCount = countFunctions(lines);
    // metrics.codeAndcommentLines = metrics.totalLines - metrics.blankLines;
    metrics.codeLines = metrics.totalLines - metrics.blankLines;
    // metrics.commentRatio = (double)((metrics1.totalCommentLines)/(metrics.totalLines));

    return metrics;
}
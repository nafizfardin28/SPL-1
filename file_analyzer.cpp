#include "metrics.h"
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
using namespace std;

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
    CommentMetrics metrics = {0, 0, 0, 0};
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
bool isBlankLine(const string &line)
{
    return std::all_of(line.begin(), line.end(), [](unsigned char c)
                       { return isspace(c); });
}

int countFunctions(const std::vector<std::string>& codeLines) {
    int functionCount = 0;

   
    std::regex functionRegex(R"(\b[a-zA-Z_][a-zA-Z0-9_:<>]*\s+[a-zA-Z_][a-zA-Z0-9_:<>]*\s*\([^)]*\)\s*(?:const)?\s*(?:\{|;))");

    bool insideFunction = false; 
    std::string functionBuffer;  

    for (const std::string& line : codeLines) {
        std::smatch match;


        functionBuffer += line + " ";

        if (std::regex_search(functionBuffer, match, functionRegex)) {
            functionCount++;
            functionBuffer.clear(); 
        }


        if (line.find('{') != std::string::npos) {
            insideFunction = true;
        } 
        else if (insideFunction && line.find('}') != std::string::npos) {
            insideFunction = false; 
            functionBuffer.clear();
        }
    }

    return functionCount;
}
sizeMetrics calculateMetrics(const vector<string> &lines)
{
    sizeMetrics metrics = {0, 0, 0, 0};
   
    for (const auto &line : lines)
    {
        metrics.totalLines++;
        string trimmedLine = line;
        if (isBlankLine(line))
            metrics.blankLines++;
    }
    metrics.functionCount = countFunctions(lines);

    return metrics; 
}

#ifndef METRICS_H
#define METRICS_H

#include <string>
#include <vector>
#include "lexer.h"
struct sizeMetrics
{
    int totalLines;
    int codeLines;
    int blankLines;
    int functionCount;
    
};

struct CommentMetrics{
    int totalLines;
    int totalCommentLines;
    int singleLineComments;
    int multiLineComments;
};
struct FileMetrics
{
    std ::string filename;
    sizeMetrics metrics;
    CommentMetrics metrics1;
    TokenCount metrics2;
};
std :: vector<std::string> readFileLines(const std::string& filename);
sizeMetrics calculateMetrics(const std::vector<std::string>& lines);
CommentMetrics calculateComments(const std::vector<std::string>& lines);
void visualizemetrics(const std::vector<FileMetrics>& allMetrics);

#endif // METRICS_H


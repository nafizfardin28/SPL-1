#ifndef HALSTEAD_H
#define HALSTEAD_H

#include <vector>
#include <string>

struct HalsteadMetrics {
    int n1; // Number of distinct operators
    int n2; // Number of distinct operands
    int N1; // Total number of operators
    int N2; // Total number of operands
    int vocabulary; // Program vocabulary
    int length; // Program length
    double calculatedLength; // Calculated program length
    double volume; // Volume
    double difficulty; // Difficulty
    double effort; // Effort
    double time; // Time
    double bugs; // Bugs
};

HalsteadMetrics calculateHalsteadMetrics(const std:: string& code);

#endif // HALSTEAD_H
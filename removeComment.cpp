#include<iostream>
#include "removeComment.h"
#include<bits/stdc++.h>
using namespace std;

string removeComments(const string& code) {
    string result;
    bool inBlockComment = false;
    bool inLineComment = false;

    for (size_t i = 0; i < code.size(); ++i) {
        if (inBlockComment) {
            if (code[i] == '*' && i + 1 < code.size() && code[i + 1] == '/') {
                inBlockComment = false;
                ++i;
            }
        } 
        else if (inLineComment) {
            if (code[i] == '\n') {
                inLineComment = false;
                result += code[i];
            }
        } 
        else {
            if (code[i] == '/' && i + 1 < code.size() && code[i + 1] == '*') {
                inBlockComment = true;
                ++i;
            } 
            else if (code[i] == '/' && i + 1 < code.size() && code[i + 1] == '/') {
                inLineComment = true;
                ++i;
            } 
            else {
                result += code[i];
            }
        }
    }

    return result;
}

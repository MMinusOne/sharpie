#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using std::vector;
using std::string;

inline vector<string> trimTokens(vector<string> tokens) {
    vector<string> trimmedTokens;
    std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(trimmedTokens), [](const string& t) {
        return !t.empty() && t != " ";
        });

    return trimmedTokens;
}
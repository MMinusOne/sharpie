#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.h"


using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::vector;

Lexer::Lexer(const std::string& sourceCode) : sourceCode(sourceCode) {}

std::vector<string> Lexer::getLines() {
    vector<string> lines;
    stringstream sourceCode(this->sourceCode);
    string line;

    while (getline(sourceCode, line)) {
        lines.push_back(line);
    }

    return lines;
}
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


// The code should of been translated to those instructions but I forgot to design it that way in the start sadly lul
enum INSTRUCTION {
    FUNCTION_DECLARATION,
    STANDARD_LIB,
    
    VARIABLE_DECLARATION,
    VARIABLE_MUTATE_CLASS_TRAIT,

    IF_STATEMENT,
    END_IF_STATEMENT,
    FOR_STATEMENT,
    FOR_END_STATEMENT,

};

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
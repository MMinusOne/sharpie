#include <iostream>
#include <string>
#include <vector>

class Lexer {
public:
    std::string sourceCode;
    Lexer(const std::string& sourceCode);

    std::vector<std::string> getLines();
};
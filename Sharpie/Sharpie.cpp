#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "Split.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::vector;

string getMainSharpieExecutionPath() {
    string executionPath;
    cout << "Enter the main sharpie exection path: ";
    std::getline(std::cin, executionPath);
    return executionPath;
}

int main() {
    string sharpiePath = getMainSharpieExecutionPath();
    ifstream file(sharpiePath);
    stringstream mainBuffer;
    mainBuffer << file.rdbuf();

    string mainCode;
    mainCode = mainBuffer.str();

    auto lexer = Lexer(mainCode);
    auto lines = lexer.getLines();

    for (const auto& line : lines) {
		std::vector<string> tokens = split(line, " ");
        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens[i] << endl;
        }
      /*  if (tokens[0] == "fn") {
            std::cout << "fn";
        }*/
    }

    return 0;
}

/**
 * Main Code
 * Map function -> scope, inner-code
 * Get main function
 * Go thru the inner-code
 *  ./examples/1-logging.sharpie
 */
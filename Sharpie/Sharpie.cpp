#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "Split.h"
#include "ScopeManager.h"
#include "Scope.h"
#include "VariableData.h";

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::vector;

string getMainSharpieExecutionPath() {
    string executionPath;
    cout << "Enter the main sharpie execution path: ";
    std::getline(std::cin, executionPath);
    return executionPath;
}

int main() {
    auto scopeManager = ScopeManager::getInstance();

    string sharpiePath = getMainSharpieExecutionPath();
    ifstream file(sharpiePath);
    stringstream mainBuffer;
    mainBuffer << file.rdbuf();

    string mainCode;
    mainCode = mainBuffer.str();

    auto lexer = Lexer(mainCode);
    auto lines = lexer.getLines();

    Scope* currentScope = nullptr;

    for (const auto& line : lines) {
		std::vector<string> tokens = split(line, " ");
        string opcode = tokens[0];
        
        if (opcode == "fn") {
            if (currentScope == nullptr) {
                currentScope = new Scope(tokens[1]);
            }
        }
        else if (opcode == "}") {
            scopeManager->addScope(currentScope->get_identifier(), currentScope);
            currentScope = nullptr;
        }
        else {

        }
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
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
        // make trim until 1 white-space
        auto splitters = std::vector<string>{ " ", "(", ")" };
        std::vector<string> tokens = split(line, splitters);
        if (tokens.empty()) continue;
        string opcode = tokens[0];
        
        if (opcode == "fn") {
            if (currentScope != nullptr) continue;
            auto fnName = tokens[1];
			currentScope = new Scope(fnName);
            
            int argumentsIndex = 2;
            bool started = true;
            while (tokens.size()-1 >= argumentsIndex) {
                auto token = tokens[argumentsIndex];
                if (token.empty()) {
                    if (!started) {
                        break;
                    }
                    argumentsIndex++;
                    started = false;
                    continue;
                }

                VariableTypes type;

                if (token == "int") {
                    type = VariableTypes::Int;
                }
                else if (token == "string") {
                    type = VariableTypes::String;
                }
                else if (token == "bool") {
                    type = VariableTypes::Bool;
                }

                string variableName = tokens[argumentsIndex + 1];
                auto variableData = new VariableData(variableName, type);
                currentScope->allocate(variableName, variableData);
                argumentsIndex += 2;
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
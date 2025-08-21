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
#include <algorithm>
#include "TypeConverter.h"
#include "Interpretter.h";
#include "trimTokens.h";
#include "StringMicroInterpreter.h";

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

bool isValidVariableName(const string& name) {
	if (name.empty() || !isalpha(name[0])) return false;
	for (char c : name) {
		if (!isalnum(c) && c != '_') return false;
	}
	return true;
}

string readFile(const string& filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		cout << "Error: Could not open file " << filePath << endl;
		return "";
	}
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

void handleFn() {

}

void initializeStandardLib() {
	auto scopeManager = ScopeManager::getInstance();

	Scope* log = new Scope("log");
	log->allocateStandardLib([](std::vector<string> args) {
		for (int i = 0; i < args.size(); i++) {
			std::cout << args[i] << " ";
		}
		});

	Scope* newLine = new Scope("newLine");
	newLine->allocateStandardLib([](std::vector<string> args) {
		std::cout << "\n";
		});

	Scope* import = new Scope("@import");
	
	import->allocateStandardLib([](std::vector<string> args) {
	vector<Scope> scopes;
	vector<string> scopeNames;
	string file;

	bool finishedImportations = false;
	for (auto arg : args) {
		if (finishedImportations) {
			file = arg.substr(1, arg.size() - 2);
			string importContents = readFile("./examples/" + file);

			auto lexer = Lexer(importContents);
			auto lines = lexer.getLines();

			Scope* currentScope = nullptr;

			for (const auto& line : lines) {
				auto splitters = std::vector<string>{ " ", "(", ")", ";" };
				std::vector<string> tokens = split(line, splitters);

				if (tokens.empty()) continue;
				auto it = std::find_if(tokens.begin(), tokens.end(), [](const string& t) {
					return !t.empty() && t != " ";
					});

				if (it == tokens.end()) continue;

				auto opcode = *it;

				if (opcode == "fn") {
					if (currentScope != nullptr) continue;
					tokens = trimTokens(tokens);
					auto fnName = tokens[1];
					currentScope = new Scope(fnName);

					int argumentsIndex = 2;
					bool started = true;
					while (tokens.size() - 1 >= argumentsIndex) {
						auto token = tokens[argumentsIndex];
						if (token.empty() || token == "{") {
							if (!started) {
								break;
							}
							argumentsIndex++;
							started = false;
							continue;
						}

						VariableTypes type = convertToType(token);

						string variableName = tokens[argumentsIndex + 1];
						if (!isValidVariableName(variableName)) {
							argumentsIndex += 2;
							continue;
						}
						auto variableData = new VariableData(variableName, type, "null");
						currentScope->allocateVariable(variableName, variableData);
						argumentsIndex += 2;
					}

				}
				else if (opcode == "}") {
					auto identifier = currentScope->get_identifier();
					ScopeManager::getInstance()->addScope(identifier, currentScope);

					currentScope = nullptr;
				}
				else {
					currentScope->addInstructions(tokens);
				}
			}

		}
		else {
			if (arg[0] == '[' && arg[arg.size() - 1] == ']') {
				arg = arg.substr(1, arg.size() - 2);
			}
			else if (arg[0] == '[') {
				arg = arg.substr(1);
			}
			else if (arg[arg.size() - 1] == ']') {
				arg = arg.substr(0, arg.size() - 1);
			}
			scopeNames.push_back(arg);
		}

		if (arg == "from") {
			finishedImportations = true;
			continue;
		}
	}
	});

	scopeManager->addScope("log", log);
	scopeManager->addScope("newLine", newLine);
	scopeManager->addScope("@import", import);
}

void handleTopLevel(vector<string> lines) {
	auto scopeManager = ScopeManager::getInstance();
	Scope* currentScope = nullptr;
	Scope* classScope = nullptr;

	for (const auto& line : lines) {
		auto splitters = std::vector<string>{ " ", "(", ")", ";" };
		std::vector<string> tokens = split(line, splitters);

		if (tokens.empty()) continue;
		auto it = std::find_if(tokens.begin(), tokens.end(), [](const string& t) {
			return !t.empty() && t != " ";
			});

		if (it == tokens.end()) continue;

		auto opcode = *it;

		if (opcode == "fn") {
			if (currentScope != nullptr) continue;
			tokens = trimTokens(tokens);
			auto fnName = tokens[1];
			currentScope = new Scope(fnName);

			int argumentsIndex = 2;
			bool started = true;
			while (tokens.size() - 1 >= argumentsIndex) {
				auto token = tokens[argumentsIndex];
				if (token.empty() || token == "{") {
					if (!started) {
						break;
					}
					argumentsIndex++;
					started = false;
					continue;
				}

				VariableTypes type = convertToType(token);

				string variableName = tokens[argumentsIndex + 1];
				if (!isValidVariableName(variableName)) {
					argumentsIndex += 2;
					continue;
				}
				auto variableData = new VariableData(variableName, type, "null");
				currentScope->allocateVariable(variableName, variableData);
				argumentsIndex += 2;
			}
		}
		else if (opcode == "class") {
			if (currentScope != nullptr) continue;
			tokens = trimTokens(tokens);
			auto className = tokens[1];
			classScope = new Scope(className);

		}
		else if (opcode == "}") {
			auto identifier = currentScope->get_identifier();
			if (classScope != nullptr) {
				currentScope->setParent(classScope);
				classScope->addFunctionScope(identifier, currentScope);
				currentScope = nullptr;
				continue;
			}
			scopeManager->addScope(identifier, currentScope);
			
			if (identifier == "main") {
				auto interpreter = new Interpreter(currentScope->getInstructions(), currentScope);
				interpreter->execute();
			}
			currentScope = nullptr;
		}
		else if (opcode == "class_end") {
			scopeManager->addScope(classScope->get_identifier(), classScope);
			classScope = nullptr;
		}
		else if (opcode[0] == '@') {
			auto macro = scopeManager->getGlobal(opcode);
			std::vector<string> relevantTokens(tokens.begin() + 1, tokens.end());
			auto argsInterpreter = new StringMicroInterpreter(relevantTokens, currentScope);
			auto args = split(argsInterpreter->execute(), { " " });
			macro->executeStandardLib(args);
		}
		else {
			if (currentScope != nullptr) {
				currentScope->addInstructions(tokens);
			}
			else {
				classScope->addInstructions(tokens);
			}
		}
	}
}

int main() {
	initializeStandardLib();

	string mainCode = readFile(getMainSharpieExecutionPath());

	auto lexer = Lexer(mainCode);
	auto lines = lexer.getLines();

	handleTopLevel(lines);

	return 0;
}

/**
 * Main Code
 * Map function -> scope, inner-code
 * Get main function
 * Go thru the inner-code
 *  ./examples/test.sharpie
 *  ./examples/1-logging.sharpie
 *  ./examples/2-if-statements.sharpie
 *  ./examples/3-loops.sharpie
 *  ./examples/4-importing.sharpie
 *  ./examples/5-fn-returns.sharpie
 */
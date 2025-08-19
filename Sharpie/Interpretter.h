#include <string>
#include <vector>
#include "Scope.h"

#pragma once
class Interpreter
{
	std::vector<std::vector<std::string>> instructions;
	Scope* scope;
	string referrerFnName;
	std::vector<string> args;
public:
	Interpreter(std::vector<std::vector<std::string>> instructions);
	Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope);
	Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope, string refererrerFnName);
	Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope, std::vector<string>& args);

	void execute();
	void handleVariable(std::vector<string>& tokens, Scope* currentScope);
	void handleFunction(std::vector<string>& tokens, Scope* currentScope);
	bool handleCondition(std::vector<string>& tokens, Scope* currentScope);

	std::string getStringOrVariableValue(std::string& code, Scope* currentScope);
	std::vector<std::string> getStringsOrVariableValues(std::vector<string>& tokens, Scope* currentScope);
	VariableData* getVariable(string name, Scope* currentScope);
};


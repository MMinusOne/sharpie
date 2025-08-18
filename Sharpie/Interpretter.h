#include <string>
#include <vector>
#include "Scope.h"

#pragma once
class Interpreter
{
	std::vector<std::vector<std::string>> instructions;
	Scope* scope;
public:
	Interpreter(std::vector<std::vector<std::string>> instructions);
	Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope);
	void execute();
};


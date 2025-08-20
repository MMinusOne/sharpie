#include <vector>
#include <string>
#include "Scope.h"

using std::string;
using std::vector;

#pragma once
class MicroInterpreter
{
	std::vector<std::string> tokens;
	Scope* currentScope = nullptr;
public:
	MicroInterpreter(vector<string>& tokens);
	MicroInterpreter(vector<string>& tokens, Scope* currentScope);
	
	string handleCondition(vector<string>& tokens);
	string handleFunction(vector<string>& tokens);
	string handleArithmetic(vector<string>& tokens);
	string execute();
};


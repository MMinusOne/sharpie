#include <vector>
#include <string>
#include "Scope.h"

using std::string;
using std::vector;

#pragma once
class StringMicroInterpreter
{
	std::vector<std::string> tokens;
	Scope* currentScope = nullptr;
public:
	StringMicroInterpreter(vector<string>& tokens);
	StringMicroInterpreter(vector<string>& tokens, Scope* currentScope);
	
	string handleCondition(vector<string>& tokens);
	string handleFunction(vector<string>& tokens);
	string handleArithmetic(vector<string>& tokens);
	string execute();
};


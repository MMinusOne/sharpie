#include <unordered_map>
#include <string>
#include "VariableData.h"
#include "FunctionData.h"

#pragma once
class Scope
{
private:
	std::unordered_map<std::string, VariableData*> variableStoreHeap;
	std::unordered_map<std::string, FunctionData*> functionStoreHeap;
	std::string identifier;
public:
	Scope(std::string identifier);
	std::string get_identifier();
	void allocateVariable(string identifier, VariableData* variableData);
	void allocateFunction(string identifier, FunctionData* variableData);
};


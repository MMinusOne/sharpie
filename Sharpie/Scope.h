#include <unordered_map>
#include <string>
#include "VariableData.h"
#include <vector>
#include <string>

#pragma once
class Scope
{
private:
	std::unordered_map<std::string, VariableData*> variableStoreHeap;
	std::vector<std::vector<string>> instructions;
	std::string identifier;
public:
	Scope(std::string identifier);
	std::string get_identifier();
	void allocateVariable(string identifier, VariableData* variableData);
	void allocateInstructions(string identifier, std::vector<std::vector<std::string>> instructions);
	std::vector<std::vector<string>> getInstructions();
};


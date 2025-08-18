#include <unordered_map>
#include <string>
#include "VariableData.h"

#pragma once
class Scope
{
private:
	std::unordered_map<std::string, VariableData> memory;
	std::string identifier;
public:
	Scope(std::string identifier);
	std::string get_identifier();
	void allocate(string identifier, VariableData* variableData);
};


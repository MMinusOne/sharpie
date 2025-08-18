#include "string";
#include "VariableTypes.h"
#include "string";
#pragma once

using std::string;

class VariableData
{
public:
	VariableData(string name, VariableTypes type);
	VariableData(string name, VariableTypes type, string value);
};


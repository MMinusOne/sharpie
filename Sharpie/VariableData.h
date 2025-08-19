#include "string";
#include "VariableTypes.h"
#pragma once

using std::string;

class VariableData
{
	string name;
	VariableTypes type;
	string value;
public:
	VariableData(string name, VariableTypes type);
	VariableData(string name, VariableTypes type, string value);

	inline string getName() { return name; }
	inline VariableTypes getType() { return type; }
	inline string getValue() { return value; }
};


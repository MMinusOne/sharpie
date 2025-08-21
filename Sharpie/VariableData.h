#pragma once
#include "string";
#include "VariableTypes.h"

class Scope;
using std::string;

class VariableData
{
	string name;
	VariableTypes type;
	string primitiveValue;
	Scope* classValue;
public:
	VariableData(string name, VariableTypes type);
	VariableData(string name, VariableTypes type, string value);

	inline string getName() { return name; }
	inline VariableTypes getType() { return type; }
	inline string getPrimitiveValue() { return primitiveValue; }
	inline Scope* getClassValue() { return classValue; }

	inline void setPrimitiveValue(string value) { this->primitiveValue = value; }
	inline void setClassValue(Scope* scope) { this->classValue = scope; }
};


#include "VariableData.h"
#include "string";

using std::string;

VariableData::VariableData(string name, VariableTypes type) {
	this->name = name;
	this->type = type;
	this->classValue = nullptr;
};
VariableData::VariableData(string name, VariableTypes type, string primitiveValue) {
	this->name = name;
	this->type = type;
	this->primitiveValue = primitiveValue;
	this->classValue = nullptr;
};
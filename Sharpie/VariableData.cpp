#include "VariableData.h"
#include "string";

using std::string;

VariableData::VariableData(string name, VariableTypes type) {
	this->name = name;
	this->type = type;
};
VariableData::VariableData(string name, VariableTypes type, string value) {
	this->name = name;
	this->type = type;
	this->value = value;
};
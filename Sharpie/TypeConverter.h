#pragma once
#include "VariableData.h"

VariableTypes convertToType(std::string type) {
	if (type == "bool") {
		return VariableTypes::Bool;
	}
	else if (type == "string") {
		return VariableTypes::String;
	}
	else if (type == "int") {
		return VariableTypes::Int;
	}

	return VariableTypes::Null;
}
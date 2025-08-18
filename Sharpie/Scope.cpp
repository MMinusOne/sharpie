#include "Scope.h"
#include "VariableData.h"

Scope::Scope(std::string identifier) {
	this->identifier = identifier;
}

std::string Scope::get_identifier() {
	return identifier;
}

void Scope::allocate(string identifier, VariableData* variableData) {
	memory[identifier] = variableData;
}
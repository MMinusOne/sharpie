#include "Scope.h"
#include "VariableData.h"

Scope::Scope(std::string identifier) {
	this->identifier = identifier;
}

std::string Scope::get_identifier() {
	return identifier;
}

void Scope::allocateVariable(string identifier, VariableData* variableData) {
	variableStoreHeap[identifier] = variableData;
}

void Scope::allocateInstructions(string identifier, std::vector<std::vector<string>> instructions) {
	this->instructions= instructions;
}

std::vector<std::vector<string>> Scope::getInstructions() {
	return this->instructions;
}
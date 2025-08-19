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

VariableData* Scope::getVariable(string identifier) {
	return variableStoreHeap[identifier];
}

void Scope::allocateInstructions(string identifier, std::vector<std::vector<string>> instructions) {
	this->instructions= instructions;
}

std::vector<std::vector<string>> Scope::getInstructions() {
	return this->instructions;
}

void Scope::allocateStandardLib(std::function<void(const std::vector<string>&)> execution) {
	standardLibInstructions = execution;
	isStandardLib = true;
}

void Scope::executeStandardLib(const std::vector<string>& data) {
	if (standardLibInstructions == nullptr) return;
	standardLibInstructions(data);
}

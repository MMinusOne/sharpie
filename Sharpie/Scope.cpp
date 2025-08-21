#include "Scope.h"
#include "VariableData.h"

Scope::Scope(std::string identifier) {
	this->identifier = identifier;
	auto selfScopeVariable = new VariableData("scope", VariableTypes::Class);
	selfScopeVariable->setClassValue(this);
	this->allocateVariable("scope", selfScopeVariable);
	parent = nullptr;
}

std::string Scope::get_identifier() {
	return identifier;
}

void Scope::allocateVariable(string identifier, VariableData* variableData) {
	variableStoreHeap[identifier] = variableData;
}

void Scope::eraseVariable(string identifier) {
	variableStoreHeap.erase(identifier);
}

Scope* Scope::getParent() {
	return this->parent;
}

VariableData* Scope::getVariable(string identifier) {
	return variableStoreHeap[identifier];
}

void Scope::setScopeReturnData(string data) {
	scopeReturnData = data;
}

void Scope::block() {
	blocked = true;
}

void Scope::unblock() {
	blocked = false;
}

bool Scope::isBlocked() {
	return blocked;
}

void Scope::allocateInstructions(std::vector<std::vector<std::string>> instructions) {
	this->instructions = instructions;
}

void Scope::addInstructions(std::vector<string> instructions) {
	this->instructions.push_back(instructions);	
}

void Scope::addFunctionScope(string fnName, Scope* fnScope) {
	functionStoreHeap[fnName] = fnScope;
}

Scope* Scope::getFunctionScope(string fnName) {
	return functionStoreHeap[fnName];
}

std::vector<std::vector<string>> Scope::getInstructions() {
	return this->instructions;
}

void Scope::allocateStandardLib(std::function<void(const std::vector<string>&)> execution) {
	if (blocked) return;
	standardLibInstructions = execution;
	isStandardLib = true;
}

void Scope::executeStandardLib(const std::vector<string>& data) {
	if (blocked) return;
	if (standardLibInstructions == nullptr) return;
	standardLibInstructions(data);
}

void Scope::setParent(Scope* parent) {
	this->variableStoreHeap.insert(parent->variableStoreHeap.begin(), parent->variableStoreHeap.end());
	this->parent = parent;
	this->depth = parent->depth + 1;
}
#include <unordered_map>
#include <string>
#include "VariableData.h"
#include <vector>
#include <string>
#include <functional>

#pragma once
class Scope
{
private:
	std::unordered_map<std::string, VariableData*> variableStoreHeap;
	std::vector<std::vector<string>> instructions;
	std::function<void(const std::vector<string>&)> standardLibInstructions = nullptr;
	bool isStandardLib = false;
	std::string identifier;
public:
	Scope(std::string identifier);
	std::string get_identifier();
	void allocateVariable(string identifier, VariableData* variableData);
	VariableData* getVariable(string identifier);
	void allocateInstructions(string identifier, std::vector<std::vector<std::string>> instructions);
	void allocateStandardLib(std::function<void(const std::vector<string>&)>);
	void executeStandardLib(const std::vector<string>& data);
	std::vector<std::vector<string>> getInstructions();
	inline bool getIsStandardLib() { return isStandardLib; }
};


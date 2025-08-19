#include "Interpretter.h"
#include <string>
#include "VariableData.h"
#include "VariableTypes.h"
#include "trimTokens.h"
#include "typeConverter.h"
#include "Scope.h"
#include "ScopeManager.h"
#include "Split.h"

Interpreter::Interpreter(std::vector<std::vector<std::string>> instructions) {
	this->instructions = instructions;
}

Interpreter::Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope) {
	this->instructions = instructions;
	this->scope = scope;
}

Interpreter::Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope, string refererrerFnName) {
	this->instructions = instructions;
	this->scope = scope;
	this->referrerFnName = refererrerFnName;
}

Interpreter::Interpreter(std::vector<std::vector<std::string>> instructions, Scope* scope, std::vector<string>& args) {
	this->instructions = instructions;
	this->scope = scope;
	this->args = args;
}

void Interpreter::execute() {
	auto scopeManager = ScopeManager::getInstance();
	Scope* currentScope = this->scope;
	for (auto& tokens : instructions) {
		if (tokens.empty()) continue;
		auto it = std::find_if(tokens.begin(), tokens.end(), [](const string& t) {
			return !t.empty() && t != " ";
			});

		if (it == tokens.end()) continue;

		auto opcode = *it;

		if (opcode == "var") {
			tokens = trimTokens(tokens);
			VariableTypes type = convertToType(tokens[1]);
			string name = tokens[2];
			string value = tokens[4];
			VariableData* variable = new VariableData(name, type, value);
			currentScope->allocateVariable(name, variable);
		}
		else {
			tokens = trimTokens(tokens);
			string fnName = tokens[0];
			auto fn = scopeManager->getGlobal(fnName);

			if (args.empty()) {
				for (int i = 1; i < tokens.size(); i++) {
					auto token = tokens[i];

					if (token[0] == '"') {
						args.push_back(token);
					}
					else {
						VariableData* varData = currentScope->getVariable(token);
						if (varData == nullptr) {
							args.push_back("null");
						};
						args.push_back(varData->getValue());
					}
				}
			}

			if (fn->getIsStandardLib()) {
				fn->executeStandardLib(args);
				args.clear();
			}
			else {
				auto fnInterpreter = new Interpreter(fn->getInstructions(), currentScope);
				fnInterpreter->execute();
			}
		}
	}
}
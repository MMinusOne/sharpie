#include "Interpretter.h"
#include <string>
#include "VariableData.h"
#include "VariableTypes.h"
#include "trimTokens.h"
#include "typeConverter.h"
#include "Scope.h"
#include "ScopeManager.h"
#include "Split.h"
#include <iostream>

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
	std::vector<Scope*> currentScopes = { this->scope };

	for (auto& tokens : instructions) {
		if (tokens.empty()) continue;
		auto it = std::find_if(tokens.begin(), tokens.end(), [](const string& t) {
			return !t.empty() && t != " ";
			});

		if (it == tokens.end()) continue;

		auto opcode = *it;

		currentScopes[currentScopes.size() - 1]->addInstructions(tokens);

		if (opcode == "var") {
			if (currentScopes[currentScopes.size() - 1]->isBlocked()) continue;
			this->handleVariable(tokens, currentScopes[currentScopes.size() - 1]);
		}
		else if (opcode == "for") {
			auto latestScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth() - 1, currentScopes);

			latestScope->addInstructions(tokens);
			if (latestScope->isBlocked()) continue;
			tokens = trimTokens(tokens);
			auto newScope = new Scope("for");
			newScope->addInstructions(tokens);
			newScope->setParent(latestScope);
			newScope->block();
			currentScopes.push_back(newScope);
		}
		else if (opcode == "for_end") {
			auto latestScope = this->findLatestScope("for", currentScopes);
			latestScope->unblock();
			vector<vector<string>> forLoopInstructions = latestScope->getInstructions();

			auto forLoopDefinition = trimTokens(forLoopInstructions[0]);

			string variableName = forLoopDefinition[1];
			int start = std::stoi(getStringOrVariableValue(forLoopDefinition[2], latestScope));
			int end = std::stoi(getStringOrVariableValue(forLoopDefinition[3], latestScope));

			VariableData* indexVariable = new VariableData(variableName, VariableTypes::Int, std::to_string(start));

			forLoopInstructions.pop_back();
			forLoopInstructions.erase(forLoopInstructions.begin());

			latestScope->allocateVariable("index", indexVariable);

			auto forLoopInterpreter = new Interpreter(forLoopInstructions, latestScope);

			for (int i = start; i < end; i++) {
				VariableData* indexVariable = new VariableData(variableName, VariableTypes::Int, std::to_string(i));
				latestScope->allocateVariable("index", indexVariable);

				forLoopInterpreter->execute();
			}

			currentScopes.pop_back();
		}
		else if (opcode == "do_if") {
			auto lastScope = findScopeDepth(currentScopes[currentScopes.size()-1]->getDepth(), currentScopes);
			if (lastScope->isBlocked()) continue;
			tokens = trimTokens(tokens);
			auto newScope = new Scope("if");
			newScope->addInstructions(tokens);
			newScope->setParent(lastScope);
			newScope->block();
			currentScopes.push_back(newScope);
		}
		else if (opcode == "if_end") {
			auto parentScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth()-1, currentScopes);
			auto lastScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth(), currentScopes);

			if (lastScope->isBlocked() && lastScope->get_identifier() != "if") continue;
			auto ifScope = currentScopes[currentScopes.size() - 1];

			currentScopes[currentScopes.size() - 1]->unblock();
			vector<vector<string>> ifInstructions = currentScopes[currentScopes.size() - 1][0].getInstructions();

			auto ifDefinition = trimTokens(ifInstructions[0]);

			std::vector<string> relevantTokens(ifDefinition.begin() + 1, ifDefinition.end());
			bool conditionState = handleCondition(relevantTokens, currentScopes[currentScopes.size() - 1]);

			ifInstructions.pop_back();
			ifInstructions.erase(ifInstructions.begin());

			auto ifInterpreter = new Interpreter(ifInstructions, currentScopes[currentScopes.size() - 1]);

			if (conditionState) {
				ifInterpreter->execute();
			}

			if (ifScope->get_identifier() == "if") currentScopes.pop_back();
		}
		else {
			if (currentScopes[currentScopes.size() - 1]->isBlocked()) continue;
			this->handleFunction(tokens, currentScopes[currentScopes.size() - 1]);
		}
	}
}

void Interpreter::handleVariable(std::vector<string>& tokens, Scope* currentScope) {
	tokens = trimTokens(tokens);
	VariableTypes type = convertToType(tokens[1]);
	string name = tokens[2];
	string value;

	for (int i = 4; i < tokens.size(); i++) {
		auto token = tokens[i];
		if (i != 4) value += " ";
		value += token;
		if (type == VariableTypes::String) {
			if (token[token.size() - 2] == '"') {
				break;
			}
		}
	}

	VariableData* variable = new VariableData(name, type, value);
	if (currentScope != nullptr) currentScope->allocateVariable(name, variable);
}

void Interpreter::handleFunction(std::vector<string>& tokens, Scope* currentScope) {
	auto scopeManager = ScopeManager::getInstance();

	tokens = trimTokens(tokens);
	string fnName = tokens[0];
	auto fn = scopeManager->getGlobal(fnName);

	if (args.empty() && !tokens.empty()) {
		std::vector<string> relevantTokens(tokens.begin() + 1, tokens.end());
		args = this->getStringsOrVariableValues(relevantTokens, currentScope);
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

bool Interpreter::handleCondition(std::vector<string>& tokens, Scope* currentScope) {
	std::vector<std::vector<string>> conditions = { {} };
	bool isValid = true;

	for (string& token : tokens) {
		string AND_OPERATOR = "&&";
		if (token == AND_OPERATOR) {
			conditions.push_back({});
		}

		conditions[conditions.size() - 1].push_back(token);
	}

	for (auto& condition : conditions) {
		auto leftSide = this->getStringOrVariableValue(tokens[0], currentScope);
		auto op = condition[1];
		auto rightSide = this->getStringOrVariableValue(tokens[2], currentScope);

		if (op == ">") {
			isValid = std::stoi(leftSide) > std::stoi(rightSide);

		}
		else if (op == "<") {
			isValid = std::stoi(leftSide) < std::stoi(rightSide);
		}
		else if (op == "<") {

		}
		else if (op == "==") {
			isValid = leftSide == rightSide;
		}
	}


	return isValid;
}

VariableData* Interpreter::getVariable(string name, Scope* currentScope) {
	auto variableData = currentScope->getVariable(name);
	if (variableData == nullptr) {
		return nullptr;
	}

	return variableData;
}

std::string Interpreter::getStringOrVariableValue(string& code, Scope* currentScope) {
	if (code[0] == '"') {
		return code.substr(1, code.size() - 1);
	}

	auto variableData = currentScope->getVariable(code);
	if (variableData == nullptr) {
		return code;
	}

	return variableData->getValue();
}

std::vector<std::string> Interpreter::getStringsOrVariableValues(std::vector<string>& tokens, Scope* currentScope) {
	bool argIsString = false;
	std::vector<std::string> args = { {} };
	for (int i = 0; i < tokens.size(); i++) {
		string token = tokens[i];

		if (argIsString) {
			if (i == tokens.size() - 1) args[0] += " ";
			if (token[token.size() - 1] == '"') {
				args.push_back(token.substr(0, token.size() - 1));
				argIsString = false;
			}
			else {
				args.push_back(token);
			}
		}
		else if (token[0] == '"') {
			if (args.size() == 0) args.push_back("");
			if (token[token.size() - 1] == '"') {
				args[0] += (token.substr(1, token.size() - 2));
				argIsString = false;
				continue;
			}
			else {
				args.push_back(token.substr(1, token.size() - 1));
			}
			argIsString = true;
		}
		else if (!argIsString) {
			VariableData* varData = currentScope->getVariable(token);
			if (varData == nullptr) {
				args.push_back("null");
			};

			args.push_back(varData->getValue());
		}
	}

	return args;
}

Scope* Interpreter::findLatestScope(const std::string& identifier, std::vector<Scope*>& currentScopes) {
	for (int i = currentScopes.size() - 1; i >= 0; i--) {
		if (currentScopes[i]->get_identifier() == identifier) {
			return currentScopes[i];
		}
	}
	return nullptr;
}

Scope* Interpreter::findScopeDepth(int depth, std::vector<Scope*>& currentScopes) {
	for(auto& currentScope: currentScopes) {
		if (currentScope->getDepth() == depth) {
			return currentScope;
			break;
	  }
	}

	return currentScopes[0];
}
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
#include "StringMicroInterpreter.h"

/**
	MAKE MICRO INTERPRETER THAT CAN HANDLE:
	"Michael"
	"Michael" lastName
	lastName

	arithmatic:
	2 + 5
	5 - 2
	a * 2
	8 * b
	operators:
	a > b
	a < b
	a == b

	"Michael" lastName "is" currentYear - dateOfBirth "years old and is he a male? " gender == "male"
*/

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

	if (!args.empty()) {
		auto scope = currentScopes[currentScopes.size() - 1];
		int argI = 0;
		for (const auto& variable : scope->getVariablesHeap()) {
			if (variable.second == nullptr || variable.first == "scope" || variable.first == "stdlib") continue;
			variable.second->setPrimitiveValue(args[argI]);
			argI++;
		}
	}

	for (auto& tokens : instructions) {
		if (tokens.empty()) continue;
		auto it = std::find_if(tokens.begin(), tokens.end(), [](const string& t) {
			return !t.empty() && t != " ";
			});

		if (it == tokens.end()) continue;

		auto opcode = *it;

		if (currentScopes.size() > 1) currentScopes[currentScopes.size() - 1]->addInstructions(tokens);

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

			auto forLoopInterpreter = new Interpreter(forLoopInstructions, latestScope, "for");

			for (int i = start; i < end; i++) {
				VariableData* indexVariable = new VariableData(variableName, VariableTypes::Int, std::to_string(i));
				latestScope->allocateVariable("index", indexVariable);

				forLoopInterpreter->execute();
			}

			currentScopes.pop_back();
		}
		else if (opcode == "do_if") {
			auto lastScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth(), currentScopes);
			if (lastScope->isBlocked()) continue;
			tokens = trimTokens(tokens);
			auto newScope = new Scope("if");
			newScope->addInstructions(tokens);
			newScope->setParent(lastScope);
			newScope->block();
			currentScopes.push_back(newScope);
		}
		else if (opcode == "if_end") {
			auto parentScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth() - 1, currentScopes);
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
		else if (opcode == "return") {
			tokens = trimTokens(tokens);
			vector<string> relevantTokens(tokens.begin() + 1, tokens.end());
			auto returnDataInterpreter = new StringMicroInterpreter(relevantTokens, currentScopes[0]);
			currentScopes[0]->setScopeReturnData(returnDataInterpreter->execute());
		}
		else {
			auto parentScope = findScopeDepth(currentScopes[currentScopes.size() - 1]->getDepth(), currentScopes);
			if (parentScope->isBlocked()) continue;
			this->handleFunction(tokens, parentScope);
		}
	}
}

void Interpreter::handleVariable(std::vector<string>& tokens, Scope* currentScope) {
	tokens = trimTokens(tokens);
	VariableTypes type = convertToType(tokens[1]);
	string name = tokens[2];
	VariableData* variable = new VariableData(name, type);

	// this is really bad, probably should refactor lul, but im too lazy, the whole project is dogshit anyway
	if (name == "mutate_class") {
		string value;
		vector<string> relevantTokens(tokens.begin() + 5, tokens.end());
		name = tokens[3];
		auto classParent = currentScope->getParent();
		if (type == VariableTypes::String) {
			auto stringMicroInterpreter = new StringMicroInterpreter(relevantTokens, currentScope);
			value = stringMicroInterpreter->execute();
		}if (type == VariableTypes::Int) {
			value = relevantTokens[0];
		}
		else if (type == VariableTypes::Bool) {

		}
		else if (type == VariableTypes::Null) {
		}
		auto variable = new VariableData(name, type, value);
		classParent->eraseVariable(name);
		classParent->allocateVariable(name, variable);
		//ScopeManager::getInstance()->updateScope(classValue->get_identifier(), classValue);
		return;
	}

	if (tokens[4] != "new") {
		string value;
		vector<string> relevantTokens(tokens.begin() + 4, tokens.end());

		if (type == VariableTypes::String) {
			auto stringMicroInterpreter = new StringMicroInterpreter(relevantTokens, currentScope);
			value = stringMicroInterpreter->execute();
		}if (type == VariableTypes::Int) {
			value = relevantTokens[0];
		}
		else if (type == VariableTypes::Bool) {

		}
		else if (type == VariableTypes::Null) {
		}

		variable->setPrimitiveValue(value);
	}
	else {
		auto scopeManager = ScopeManager::getInstance();
		auto classRef = scopeManager->getGlobal(tokens[5]);

		auto classFieldsInterpreter = new Interpreter(classRef->getInstructions(), classRef);
		classFieldsInterpreter->execute();

		variable->setClassValue(classRef);
	}

	if (currentScope != nullptr) currentScope->allocateVariable(name, variable);
}

void Interpreter::handleFunction(std::vector<string>& tokens, Scope* currentScope) {
	auto scopeManager = ScopeManager::getInstance();
	std::vector<std::string> fnArgs;

	tokens = trimTokens(tokens);
	auto paths = split(tokens[0], { "." });
	string fnName;

	Scope* fn;

	if (paths.size() == 1) {
		fnName = paths[0];
		fn = scopeManager->getGlobal(fnName);
	}
	else {
		fnName = paths[1];
		if (paths[0] == "stdlib") {
			auto stdlibScope = scopeManager->getGlobal("stdlib");
			fn = stdlibScope->getFunctionScope(fnName);
		}
		else if (paths[0] == "httplib") {
			auto stdlibScope = scopeManager->getGlobal("httplib");
			fn = stdlibScope->getFunctionScope(fnName);
		}
		else {
			auto varRef = currentScope->getVariable(paths[0]);
			auto classValue = varRef->getClassValue();
			fn = classValue->getFunctionScope(fnName);
		}
	}

	if (!tokens.empty()) {
		std::vector<string> relevantTokens(tokens.begin() + 1, tokens.end());
		fnArgs = this->getStringsOrVariableValues(relevantTokens, currentScope);
	}
	if (fn->getIsStandardLib()) {
		if (fnName[0] != '@') {
			fn->executeStandardLib(fnArgs);
			fnArgs.clear();
		}
	}
	else {
		auto fnInterpreter = new Interpreter(fn->getInstructions(), fn, fnArgs);
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

//int Interpreter::handleArithmetic(std::vector<string>&tokens, Scope* currentScope) {
//	
//}

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

	return variableData->getPrimitiveValue();
}

std::vector<std::string> Interpreter::getStringsOrVariableValues(std::vector<string>& tokens, Scope* currentScope) {
	bool argIsString = false;
	std::vector<std::string> args = {  };
	for (int i = 0; i < tokens.size(); i++) {
		string token = tokens[i];

		if (argIsString) {
			if (token[token.size() - 1] == '"') {
				args.push_back(token.substr(0, token.size() - 1));
				argIsString = false;
			}
			else {
				args.push_back(token);
			}
		}
		else if (token[0] == '"') {
			if (token[token.size() - 1] == '"') {
				args.push_back(token.substr(1, token.size() - 2));
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
				continue;
			};

			args.push_back(varData->getPrimitiveValue());
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
	for (auto& currentScope : currentScopes) {
		if (currentScope->getDepth() == depth) {
			return currentScope;
			break;
		}
	}

	return currentScopes[0];
}
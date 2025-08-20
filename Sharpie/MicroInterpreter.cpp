#include "MicroInterpreter.h"
#include <vector>
#include <string>
#include "Scope.h"
#include  "ScopeManager.h"	
#include "Interpretter.h";

MicroInterpreter::MicroInterpreter(vector<string>& tokens) {
	this->tokens = tokens;
};

MicroInterpreter::MicroInterpreter(vector<string>& tokens, Scope* scope) {
	this->tokens = tokens;
	this->currentScope = scope;
};

string MicroInterpreter::handleArithmetic(vector<string>& tokens) {
	return "";
}

string MicroInterpreter::handleCondition(vector<string>& tokens) {
	return "";
}

string MicroInterpreter::handleFunction(vector<string>& tokens) {
	return "";
}

string MicroInterpreter::execute() {
	string output;

	bool isInFunctionArgs = false;
	bool isInString = false;

	string tempBlock;
	std::vector<std::string> fnArgs;
	Scope* fnScope = nullptr;

	for (auto& token : tokens) {
		if (token[0] == '"' && token[token.size() - 1] == '"') {
			tempBlock += token.substr(1, token.size() - 2);
			if (!isInFunctionArgs) {
				output += tempBlock;
			}
			else {
				fnArgs.push_back(tempBlock);
			}
			tempBlock.clear();
			continue;
		}

		if (token[0] == '"') {
			tempBlock += token.substr(1, token.size() - 1);
			tempBlock += " ";
			fnArgs.push_back(tempBlock);
			isInString = true;
			continue;
		}


		if (token[token.size() - 1] == '"') {
			tempBlock += token.substr(0, token.size() - 1);
			if (!isInFunctionArgs) {
				output += tempBlock;
			}
			else {
				fnArgs.push_back(tempBlock);
			}
			tempBlock.clear();
			isInString = false;
			continue;
		}

		if (isInString) {
			tempBlock += token;
			tempBlock += " ";
			continue;
		}

		if (token[0] == '<') {
			isInFunctionArgs = true;
			continue;
		}

		if (token[0] == '>') {
			auto fnInterpreter = new Interpreter(fnScope->getInstructions(), fnScope, fnArgs);
			fnInterpreter->execute();
			output += fnScope->getScopeReturnData();
			isInFunctionArgs = false;
			fnScope = nullptr;
			continue;
		}


		auto scope = ScopeManager::getInstance()->getGlobal(token);

		if (scope != nullptr) {
			fnScope = scope;
			continue;
		}

		auto varData = currentScope->getVariable(token);

		if (varData == nullptr) {
			tempBlock += varData->getValue();
			if (isInFunctionArgs) {
				fnArgs.push_back(tempBlock);
			}
		}
	}

	return output;
}
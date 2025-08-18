#include "Interpretter.h"
#include <string>

Interpretter::Interpretter(std::vector<std::vector<std::string>> instructions) {
	this->instructions = instructions;
}

void Interpretter::execute() {

    //else if(opcode == "var") {
        //tokens = trimTokens(tokens);
        //VariableTypes type = convertToType(tokens[1]);
        //string name = tokens[2];
        //string value = tokens[4];
        //VariableData* variable = new VariableData(name, type, value);
        //currentScope->allocate(name, variable);
    //}
    //else {
        //tokens = trimTokens(tokens);
        //string fnName = tokens[0];
        //auto fn = scopeManager->getGlobal(fnName);
    //}
}
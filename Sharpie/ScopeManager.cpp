#include "ScopeManager.h"
#include "Scope.h"
#include "string";

ScopeManager::ScopeManager(const ScopeManager&) = delete;

ScopeManager* ScopeManager::getInstance() {
	if (instance == nullptr) {
		instance = new ScopeManager();
	}

	return instance;
}

void ScopeManager::addScope(std::string scopeIdentifier, Scope* scope) {
	scopeMemory[scopeIdentifier] = scope;
}
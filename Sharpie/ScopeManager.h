#include <unordered_map>
#include "Scope.h"

#pragma once
class ScopeManager
{
private:
	static ScopeManager* instance;
	std::unordered_map<std::string, Scope*> scopeMemory;
	ScopeManager();
public:
	ScopeManager& operator=(const ScopeManager&) = delete;
	static ScopeManager* getInstance();
	void addScope(std::string scopeIdentifier, Scope* scope);
};


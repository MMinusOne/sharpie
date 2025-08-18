#include <unordered_map>
#include "Scope.h"

#pragma once
class ScopeManager
{
private:
	static ScopeManager* instance;
	std::unordered_map<std::string, Scope*> scopeMemory;
	ScopeManager() = default;
	ScopeManager(const ScopeManager&) = delete;
	ScopeManager& operator=(const ScopeManager&) = delete;
public:
	static ScopeManager* getInstance();
	void addScope(std::string scopeIdentifier, Scope* scope);
};


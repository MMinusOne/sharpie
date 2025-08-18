#include "ScopeManager.h"

ScopeManager::ScopeManager(const ScopeManager&) = delete;

ScopeManager* ScopeManager::getInstance() {
	if (instance == nullptr) {
		instance = new ScopeManager();
	}

	return instance;
}
#pragma once
class ScopeManager
{
private:
	static ScopeManager* instance;
	ScopeManager();
public:
	ScopeManager& operator=(const ScopeManager&) = delete;
	static ScopeManager* getInstance();
};


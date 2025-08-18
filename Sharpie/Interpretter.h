#include <string>
#include <vector>

#pragma once
class Interpretter
{
	std::vector<std::vector<std::string>> instructions;
public:
	Interpretter(std::vector<std::vector<std::string>> instructions);
	void execute();
};


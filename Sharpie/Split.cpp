#include "Split.h"

#include <vector>
#include <string>

std::vector<std::string> split(std::string input, std::string splitter) {
	std::vector<std::string> splitted;
	std::string temp;
	size_t pos = 0;
	size_t splitterLength = splitter.length();

	while ((pos = input.find(splitter)) != std::string::npos) {
		temp = input.substr(0, pos);
		splitted.push_back(temp);
		input.erase(0, pos + splitterLength);
	}

	splitted.push_back(input);

	return splitted;
}
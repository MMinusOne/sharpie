#include "Split.h"

#include <vector>
#include <string>

std::vector<std::string> split(std::string input, const std::vector<std::string> splitters) {
    std::vector<std::string> splitted;
    std::string temp;
    size_t pos = 0;

    while (!input.empty()) {
        size_t minPos = std::string::npos; 
        std::string foundSplitter;

        for (const std::string& splitter : splitters) {
            size_t splitterPos = input.find(splitter);
            if (splitterPos < minPos) {
                minPos = splitterPos;
                foundSplitter = splitter;
            }
        }

        if (minPos == std::string::npos) {
            splitted.push_back(input);
            break;
        }

        temp = input.substr(0, minPos);
        splitted.push_back(temp);

        input.erase(0, minPos + foundSplitter.length());
    }

    return splitted;
}
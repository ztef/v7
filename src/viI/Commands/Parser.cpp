#include "Parser.h"
#include <regex>
#include <iostream>

Parser::Parser() {}

void Parser::setCommandRegex(const std::string& regexPattern) {
    commandRegex = std::regex(regexPattern);
}

std::vector<std::string> Parser::tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;

    // Regular expression to match the SET command format
    std::regex setRegex(R"(^\s*SET\s+(\w+)\s*=\s*(.*?)(?:\s*;\s*)?$)");
    // Regular expression to match custom command format (e.g., A B;)
    std::regex customCommandRegex(R"(^\w+(?:\s+[\w.]+)*\s*(?:;\s*)?$)");

    std::smatch match;
    if (std::regex_match(line, match, setRegex)) {
        tokens.push_back("SET");
        tokens.push_back(match.str(1)); // Variable name
        tokens.push_back("=");
        std::istringstream iss(match.str(2));
        std::string valueToken;
        while (iss >> valueToken) {
            tokens.push_back(valueToken);
        }
    } else if (std::regex_match(line, match, customCommandRegex)) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            // Remove the trailing semicolon if it exists
            if (token.back() == ';') {
                token = token.substr(0, token.size() - 1);
            }
            tokens.push_back(token);
        }
    } else {
        // If no match, treat the entire line as a single token
        std::cout << "No reconocido" << std::endl;
        tokens.push_back(line);
    }

    return tokens;
}




std::regex Parser::getCommandRegex() {
    return commandRegex;
}

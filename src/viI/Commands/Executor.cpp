#include "Executor.h"
#include <iostream>
#include <regex>

Executor::Executor() {}


void Executor::setCommandRegex(const std::string& regexPattern) {
    commandRegex = std::regex(regexPattern);
}



std::string Executor::executeCommand(const std::vector<std::string>& tokens, std::map<std::string, std::string>& variables) {
    std::string command = tokens[0];
    std::string result;

    std::cout << "COMANDO TOKENIZADO :" << command << std::endl;

    if (command == "SET") {
        result = executeSetCommand(tokens, variables);
    } else if (command == "SHOW") {
         result = executeShowCommand(variables);
    } else if (std::regex_match(command, commandRegex)) {
         result =  executeCustomCommand(tokens);
    } else {
        result =  "Unknown command: ";
    }
    return result;
}

std::string Executor::executeSetCommand(const std::vector<std::string>& tokens, std::map<std::string, std::string>& variables) {
    if (tokens.size() == 4 && tokens[2] == "=") {
        const std::string variableName = tokens[1];
        const std::string value = tokens[3];

        // Check if the value is quoted (string) or not (variable or number)
        if (value.front() == '"' && value.back() == '"') {
            // Remove quotes from the string value
            variables[variableName] = value.substr(1, value.size() - 2);
        } else {
            // If not quoted, assign the value as it is
            variables[variableName] = value;
        }
    } else {
        std::cout << "Invalid SET command format." << tokens[0] << tokens[1] << tokens[2] << std::endl;
        
    }
    return "SET ok";
}

std::string Executor::executeShowCommand(const std::map<std::string, std::string>& variables) {
    std::cout << "Variables setted:" << std::endl;
    for (const auto& pair : variables) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }
     return "SHOW ok";
}

std::string Executor::executeCustomCommand(const std::vector<std::string>& tokens) {
    std::cout << "Executing custom command: " << tokens[0];
    if (tokens.size() > 1) {
        std::cout << " with arguments:";
        for (size_t i = 1; i < tokens.size(); ++i) {
            std::cout << " " << tokens[i];
        }
    }
    return "CUSTOM ok";
}

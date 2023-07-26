#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <vector>
#include <map>
#include <regex>

class Executor {
public:
    Executor();
    void setCommandRegex(const std::string& regexPattern);
    std::string executeCommand(const std::vector<std::string>& tokens, std::map<std::string, std::string>& variables);

private:
    std::string executeSetCommand(const std::vector<std::string>& tokens, std::map<std::string, std::string>& variables);
    std::string executeShowCommand(const std::map<std::string, std::string>& variables);
    std::string executeCustomCommand(const std::vector<std::string>& tokens);
    std::regex commandRegex;
};

#endif // EXECUTOR_H

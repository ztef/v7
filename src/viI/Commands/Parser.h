#ifndef PARSER_H
#define PARSER_H

#include <regex>
#include <vector>
#include <string>

class Parser {
public:
    Parser();
    void setCommandRegex(const std::string& regexPattern);
    std::vector<std::string> tokenizeLine(const std::string& line);
    std::regex getCommandRegex();

private:
    std::regex commandRegex;
};

#endif // PARSER_H

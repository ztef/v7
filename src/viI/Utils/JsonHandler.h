#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Json.hpp"

using json = nlohmann::json;




class JSONHandler {
public:
    JSONHandler() = default;
    ~JSONHandler() = default;

    bool LoadJSONFromFile(const std::string& filename) {
        // Read the JSON data from the file.
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open the JSON file: " << filename << std::endl;
            return false;
        }

        try {
            file >> json_data_;
        } catch (const json::parse_error& e) {
            std::cerr << "Error: Failed to parse the JSON file: " << filename << ". Reason: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    json GetJSONObject() const {
        // Return the stored JSON object.
        return json_data_;
    }

private:
    json json_data_;
};
#pragma once
#include <iostream>
#include "../Utils/JsonHandler.h"

class DataStore {

    public:

             json json_object;


            void setJsonObject(json j){
                json_object = j;

                std::cout << "JSON Object:" << std::endl;
                std::cout << json_object.dump(4) << std::endl;
            }



            void showData(){
                 std::cout << "JSON Object:" << std::endl;
                 std::cout << json_object[0] << std::endl;

                // Check if the top-level element is an array.
                if (json_object.is_array()) {
                    // Iterate through the JSON array and print each element.
                    for (const auto& element : json_object) {
                        std::cout << "Array Element: " << element["Agrupador"] << std::endl;
                    }
                } else {
                    std::cout << "The JSON does not contain an array." << std::endl;
        }



            }


};
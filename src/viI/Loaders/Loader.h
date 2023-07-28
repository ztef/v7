#include <iostream>
#include "../Utils/JsonHandler.h"
#include "../DataStore/DataStore.h"

class Loader {

public:

    static void loadJson(DataStore * datastore){

         std::cout << "CARGANDO DATA" << std::endl;    
    
         JSONHandler json_handler;
         std::string filename = "example.json";

        if (json_handler.LoadJSONFromFile(filename)) {
            datastore->setJsonObject(json_handler.GetJSONObject());          
        }
    
    }



};
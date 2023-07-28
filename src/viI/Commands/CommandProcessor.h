#pragma once
#include "../Registry/Registry.h"
#include "../Actors/Actor.h"
#include "../Creators/ActorCreator.h"
#include "Parser.h"
#include "Executor.h"

#include <regex>



class CommandProcessor {


public:


 std::map<std::string, std::string> variables;
 
 Parser parser;
 Executor executor;



void setRegistry(Registry * in_registry){
  executor.registry = in_registry;
}



std::string parse(std::string input){
 
                                            
      std::string result;

       parser.setCommandRegex(R"(^\s*(SET\s+(\w+)\s*=\s*(?:\"([^\"]*)\"|(\w+)|(-?\d+(?:\.\d+)?))|(\w+)\s+(\w+)|SHOW)\s*;?\s*$)");
       executor.setCommandRegex(R"(^\s*(SET\s+(\w+)\s*=\s*(?:\"([^\"]*)\"|(\w+)|(-?\d+(?:\.\d+)?))|(\w+)\s+(\w+)|SHOW)\s*;?\s*$)");
        
    
        std::cout << "Recibiendo :" << input << std::endl;

         std::vector<std::string> tokens = parser.tokenizeLine(input);
     
        if (tokens.empty()) {
             
        } else {

                        std::cout << "Ejecutando :" << input << std::endl;
                        result = executor.executeCommand(tokens, variables);

        }

        return result;
}


/*

std::string parseold(std::string input){
        std::cout << input << std::endl; 

       if(input == "SET SCENE = XYZ"){

                registry->scenecontroller->setScene(1);

       }




        if(input == "ADD"){


                        // CREA UN ACTOR B
                        auto bb =  ActorCreator::create(registry->builder,registry->stateInfo, 10 ,0,"STATIC");
                        registry->radial_scene->addChild(true,bb->node);

        } else {


        }




        return "ok";
}
*/



};
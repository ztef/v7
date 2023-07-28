#pragma once
#include <vsg/all.h>
#include "../Scenes/Scene.h"
#include "../Visitors/FindVertexData.h"
#include "TextCreator.h"

#include <stdio.h>
#include <vsgXchange/all.h>

class SceneCreator {

public:


static Scene* create(vsg::ref_ptr<vsg::Builder> builder, vsg::StateInfo in_stateInfo, float x, float y , std::string mode){

            vsg::GeometryInfo geomInfo;

            vsg::StateInfo stateInfo;

               stateInfo.lighting = true;

            geomInfo.dx.set(20000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 20000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 250000.0f);


            geomInfo.position.x = x;
            geomInfo.position.y = y;

             

            auto geom = builder->createCylinder(geomInfo, stateInfo);
                 //geom->setValue("name", "CAJA");


           auto scene = new Scene();
           scene->node->addChild(geom);

          if(mode == "DYNAMIC"){


                         // Hace que este actor sea manipulable (DYNAMIC DATA)
                        size_t numVertices = 0;
                        auto verticesList = vsg::visit<FindVertexData>(geom).getVerticesList();
                       
                            for(auto& vertices : verticesList)
                            {
                                vertices->properties.dataVariance = vsg::DYNAMIC_DATA;
                                numVertices += vertices->size();
                            }



                          scene->verticesList = verticesList;
                          scene->isDynamic = true;

          } else {
                scene->isDynamic = false;

          }


         

          return (scene);


}



static Scene* createPlane(vsg::ref_ptr<vsg::Builder> builder, vsg::StateInfo in_stateInfo, float n, float m , std::string mode, vsg::ref_ptr<vsg::Font> fontx, vsg::ref_ptr<vsg::Options> local_optionsx ){



             
              auto options = vsg::Options::create();
                options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
                options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

                // Usa vsgXchange para lectura de archivos en diferentes formatos (autocad, obj etc)
                options->add(vsgXchange::all::create());




             auto font_filename = std::string("./fonts/times.vsgb");
             auto font = vsg::read_cast<vsg::Font>(font_filename, options);


            vsg::GeometryInfo geomInfo;

            vsg::StateInfo stateInfo;
                
                stateInfo.wireframe = false;
                stateInfo.instance_colors_vec4 = true;
                stateInfo.lighting = true;

            geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);


            auto colors = vsg::vec4Array::create(1);

            colors->at(0).set(1.0f, 0.0f, 0.0f, 1.0f);

            geomInfo.colors = colors;

            geomInfo.position.x = geomInfo.position.x - (geomInfo.dx.x * 1.1f * n) / 2;
            geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * m) / 2;
            geomInfo.position.z = 0;

          
           auto scene = new Scene();


         for (int i = 0; i < n; i++)
            {
                geomInfo.position += geomInfo.dx * 1.1f;
                for (int j = 0; j < m; j++)
                {
                    geomInfo.position += geomInfo.dy * 1.1f;
                    scene->node->addChild(builder->createQuad(geomInfo, stateInfo));
                }
                geomInfo.position = geomInfo.position - (geomInfo.dy * (1.1f * m));
            }

            // Labels en X

            

            geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);

            geomInfo.position.x = geomInfo.position.x - (geomInfo.dx.x * 1.1f * n);
            // geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * m);
            geomInfo.position.z = 0;

            for (int i = 0; i < n; i++)
            {
                geomInfo.position += geomInfo.dx * 1.1f;
                auto text = TextCreator::create(std::to_string(i), geomInfo.position.x, geomInfo.position.y, 100000, 500000, font, options);
                
                scene->node->addChild(text);
            
            }

             // Labels en Y
            geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);

            geomInfo.position.x = geomInfo.position.x + (geomInfo.dx.x * 1.1f);
    
            geomInfo.position.z = 0;

            for (int j = 0; j < n; j++)
            {
                geomInfo.position += geomInfo.dy * 1.1f;
                auto text = TextCreator::create(std::to_string(j), geomInfo.position.x, geomInfo.position.y, 100000, 500000, font, options);
               
               
                scene->node->addChild( text);
            }

            


          return (scene);


}




};



   
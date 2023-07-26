#pragma once
#include <vsg/all.h>
#include "../Actors/Actor.h"
#include "../Visitors/FindVertexData.h"

#include <stdio.h>

class ActorCreator {

public:


static Actor* create(vsg::ref_ptr<vsg::Builder> builder, vsg::StateInfo in_stateInfo, float x, float y , std::string mode){

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


           auto actor = new Actor();
           actor->node = geom;

          if(mode == "DYNAMIC"){


                         // Hace que este actor sea manipulable (DYNAMIC DATA)
                        size_t numVertices = 0;
                        auto verticesList = vsg::visit<FindVertexData>(geom).getVerticesList();
                       
                            for(auto& vertices : verticesList)
                            {
                                vertices->properties.dataVariance = vsg::DYNAMIC_DATA;
                                numVertices += vertices->size();
                            }



                          actor->verticesList = verticesList;
                          actor->isDynamic = true;

          } else {
                actor->isDynamic = false;

          }


         

          return (actor);


}

};



   
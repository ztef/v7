
#pragma once
#include <vsg/all.h>
#include "../Actors/Actor.h"



class TransformEngine {





    public : 

       void transform(){

       }



       void transforma(vsg::ref_ptr<Actor> a, double frameCount, double radius){
                 for(auto& vertices : a->verticesList)
                {
                    for(auto& v : *vertices)
                    {
                        v.x += (sin(vsg::PI * frameCount / 180.0) * radius * 0.001);
                    }
                    vertices->dirty();
                }
       }

};
#pragma once
#include <vsg/all.h>




class Scene  {

public:

    Scene(){
        node = vsg::Group::create();
    };

    vsg::ref_ptr<vsg::Group> node;
    bool isDynamic = false;
    std::vector<vsg::ref_ptr<vsg::vec3Array>>  verticesList;




};
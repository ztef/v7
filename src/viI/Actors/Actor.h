#pragma once
#include <vsg/all.h>




class Actor  {

public:

    Actor(){};

    vsg::ref_ptr<vsg::Node> node;
    bool isDynamic = false;
    std::vector<vsg::ref_ptr<vsg::vec3Array>>  verticesList;




};
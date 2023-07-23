#pragma once
#include <vsg/all.h>




class Actor : public vsg::Object {

public:

    vsg::ref_ptr<vsg::Node> node;
    bool isDynamic;
    std::vector<vsg::ref_ptr<vsg::vec3Array>>  verticesList;




};
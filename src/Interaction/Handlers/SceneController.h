#pragma once
#include <iostream>
#include <vsg/all.h>
#include "MouseHandler.h"


class SceneController {

public:

    vsg::ref_ptr<vsg::Switch> s_node;
    vsg::ref_ptr<MouseHandler> mouseHandler;

    void setSwitchNode(vsg::ref_ptr<vsg::Switch> snode){
            s_node = snode;
    }


    void setMouseHandler(vsg::ref_ptr<MouseHandler> hh)
    {
        mouseHandler = hh;
    }



    void setScene(int s){
        std::cout << "Escena seleccionada :" << s << std::endl;
        s_node->setSingleChildOn((size_t) s);
        mouseHandler->trackball_active = s;
    }

};

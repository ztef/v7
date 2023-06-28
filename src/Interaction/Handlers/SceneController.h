#pragma once
#include <iostream>
#include <vsg/all.h>


class SceneController {

public:

    vsg::ref_ptr<vsg::Switch> s_node;

    void setSwitchNode(vsg::ref_ptr<vsg::Switch> snode){
            s_node = snode;
    }


    void setScene(int s){
        std::cout << "Escena seleccionada :" << s << std::endl;
        s_node->setSingleChildOn((size_t) s);
    }

};

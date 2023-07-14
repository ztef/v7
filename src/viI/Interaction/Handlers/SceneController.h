#pragma once
#include <iostream>
#include <vsg/all.h>
#include "MouseHandler.h"


class SceneController {

public:

    vsg::ref_ptr<vsg::Switch> t_node;   // nodo de tipos (mundo geografico, mundo abstracto)
    vsg::ref_ptr<vsg::Switch> a_node;   // mundo geografico
    vsg::ref_ptr<MouseHandler> mouseHandler;

    void setSwitchNode(vsg::ref_ptr<vsg::Switch> snode){
            t_node = snode;
    }

    void setSwitchAbstract(vsg::ref_ptr<vsg::Switch> snode){
            a_node = snode;
    }

     


    void setMouseHandler(vsg::ref_ptr<MouseHandler> hh)
    {
        mouseHandler = hh;
        mouseHandler->trackball_active =1;
    }


    // 0:modo geografico, 1:modo abstracto
    void setMode(int m){
        t_node->setSingleChildOn((size_t) m);
        mouseHandler->trackball_active = m;
    }

     void setAbstractScene(int s){
        
        a_node->setSingleChildOn((size_t) s);
        
    }

    void setScene(int s){
        std::cout << "Escena seleccionada :" << s << std::endl;

        if(s == 5){
            setMode(0);
        } else {
            setMode(1);

            setAbstractScene(s);
        }

     
    }

};

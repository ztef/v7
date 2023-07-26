#pragma once
#include <vsg/all.h>
#include "../Interactors/Handlers/SceneController.h"

class Registry {

public:


 vsg::ref_ptr<vsg::Builder> builder;
 vsg::StateInfo stateInfo;

 SceneController * scenecontroller;

 vsg::ref_ptr<vsg::Group> earth_scene;
 vsg::ref_ptr<vsg::Switch> abstract_nodes;
 vsg::ref_ptr<vsg::Switch> xyz_scene;
 vsg::ref_ptr<vsg::Switch> radial_scene;

};
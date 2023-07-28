#pragma once
#include <vsg/all.h>
#include "../Interactors/Handlers/SceneController.h"
#include "../Creators/Creator.h"
#include "../DataStore/DataStore.h"

class Registry {

public:


 vsg::ref_ptr<vsg::Builder> builder;
 vsg::StateInfo stateInfo;

 vsg::ref_ptr<vsg::Font> font;
 vsg::ref_ptr<vsg::Options> in_options;

 SceneController * scenecontroller;
 Creator * creator;

 vsg::ref_ptr<vsg::Group> earth_scene;
 vsg::ref_ptr<vsg::Switch> abstract_nodes;
 vsg::ref_ptr<vsg::Switch> xyz_scene;
 vsg::ref_ptr<vsg::Switch> radial_scene;
 vsg::ref_ptr<vsg::Switch> cylindrical_scene;

 vsg::ref_ptr<vsg::Viewer> viewer;


 DataStore * datastore;



};
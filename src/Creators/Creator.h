
#include <stdio.h>
#include <iostream>
#include <vsg/all.h>
#include "RawGeometry.h"
#include "PipeLineProcessor.h"



enum primitiveTypes {
      box,
      plane,
      cylinder,
      sphere
};

class Creator {
    public:


        vsg::ref_ptr<vsg::Builder> builder;
        vsg::StateInfo stateInfo;
        vsg::GeometryInfo geomInfo;

        Creator();

        vsg::ref_ptr<vsg::Switch> scene;
        vsg::ref_ptr<vsg::Options> local_options;

        void init();
        void setScene(vsg::ref_ptr<vsg::Switch> in_scene);
        void setOptions(vsg::ref_ptr<vsg::Options> in_options);
        
        void createFromFile(std::string filename);
        void createPrimitive(std::string primitive);
        void createRaw(RawGeometry raw);
        void createPlane();

        PipeLineProcessor plProcessor;


};

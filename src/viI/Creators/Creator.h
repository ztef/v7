
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
        vsg::ref_ptr<vsg::Font> font;

        void init();
        void setScene(vsg::ref_ptr<vsg::Switch> in_scene);
        void resetPositions();
        void setOptions(vsg::ref_ptr<vsg::Options> in_options, vsg::ref_ptr<vsg::Font> in_font);
        
        void createFromFile(std::string filename);
        void createPrimitive(std::string primitive);
        void createRaw(RawGeometry raw);
        void createPlane(int m, int n);
        void createRadial(int r, int n);
        void createRadialDepth(int r, int n);
        void createSpherical(int r, int n);
        void createCylindrical(int r, int n);
        PipeLineProcessor plProcessor;


};

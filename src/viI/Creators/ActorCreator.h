#include <vsg/all.h>



class ActorCreator {

public:


static vsg::ref_ptr<vsg::Node> create(vsg::ref_ptr<vsg::Builder> builder, vsg::StateInfo stateInfo, float x, float y ){

            vsg::GeometryInfo geomInfo;

            geomInfo.dx.set(20000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 20000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 250000.0f);

             

            auto actor = builder->createCylinder(geomInfo, stateInfo);
                 actor->setValue("name", "CAJA");



            return actor;


}

};
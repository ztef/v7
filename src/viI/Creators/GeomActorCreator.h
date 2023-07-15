#include <vsg/all.h>



class GeomActorCreator {

public:


static vsg::ref_ptr<vsg::Node> create(vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel, vsg::ref_ptr<vsg::Builder> builder, vsg::StateInfo stateInfo, float lat, float lng ){

            vsg::GeometryInfo geomInfo;

            geomInfo.dx.set(20000.0f, 0.0f, 0.0f);
            geomInfo.dy.set(0.0f, 20000.0f, 0.0f);
            geomInfo.dz.set(0.0f, 0.0f, 250000.0f);

            geomInfo.transform = ellipsoidModel->computeLocalToWorldTransform({lat, lng, 0.0});

            auto box = builder->createCylinder(geomInfo, stateInfo);
                 box->setValue("name", "CAJA");



            return box;


}

};
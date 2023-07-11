
#include <vsg/all.h>



// Funcion que crea una camara para la escena :

vsg::ref_ptr<vsg::Camera> createCameraForScene(vsg::Node *scenegraph, int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    scenegraph->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 10.6;
    double nearFarRatio = 0.0001;

    // set up the camera
    auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0),
                                      centre, vsg::dvec3(0.0, 0.0, 1.0));

    auto perspective = vsg::Perspective::create(30.0, static_cast<double>(width) / static_cast<double>(height),
                                                nearFarRatio * radius, radius * 4.5);

    auto viewportstate = vsg::ViewportState::create(x, y, width, height);

    return vsg::Camera::create(perspective, lookAt, viewportstate);
}


vsg::ref_ptr<vsg::Camera> createCameraForGeoScene(vsg::Node *earth_scene, int32_t x, int32_t y, uint32_t width, uint32_t height, auto window, auto ellipsoidModel)
{
    // Bounds y Parametros 

                vsg::ComputeBounds computeBounds;
                earth_scene->accept(computeBounds);

                vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
                double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.9;
                double nearFarRatio = 0.001;

                
                vsg::ref_ptr<vsg::LookAt> lookAt;
                vsg::ref_ptr<vsg::ProjectionMatrix> perspective;

                lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

                perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, 0.0);

                //  Crea Camara :

                return  vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));
}

#include <vsg/all.h>




class LightCreator {

public:

static vsg::ref_ptr<vsg::DirectionalLight> create(){
   
     auto directionalLight1 = vsg::DirectionalLight::create();
                            directionalLight1->name = "directional";
                            directionalLight1->color.set(1.0, 1.0, 1.0);
                            directionalLight1->intensity = 0.99;
                            directionalLight1->direction.set(0.0, -1.0, -1.0);

    return  directionalLight1;

}





};
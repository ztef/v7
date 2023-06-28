#include "Creator.h"



Creator::Creator(){

        builder = vsg::Builder::create();


        // POSICION EN EL MUNDO

        geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
        geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
        geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);

};

void Creator::init(){

    std::cout << "CREATOR CREADO...." << std::endl;
};


void Creator::setScene(vsg::ref_ptr<vsg::Switch> in_scene){
    scene = in_scene;
};

void Creator::setOptions(vsg::ref_ptr<vsg::Options> in_options){
    local_options = in_options;
};

void Creator::createFromFile(std::string in_filename){
     vsg::Path filename = in_filename;
                    
                    auto loaded_scene = vsg::read_cast<vsg::Node>(filename, local_options);
         
                    if (loaded_scene)
                    {
                        // AGREGA EL MODELO A LA ESCENA
                         scene->addChild(true,loaded_scene);
                        
                    }
};

void Creator::createPrimitive(std::string primitive){
  if(primitive == "box"){

     geomInfo.position += geomInfo.dx * 1.5f;
     auto box =  builder->createBox(geomInfo, stateInfo);
     box->setValue("name","CAJA");
     scene->addChild(true,box);

  } else if (primitive == "plane")
  {

     geomInfo.position += geomInfo.dx * 1.5f;
     scene->addChild(true, builder->createQuad(geomInfo, stateInfo));

  }

 };

 void Creator::createPlane(){



    geomInfo.position.x =geomInfo.position.x - (geomInfo.dx.x * 1.1f * 4.0f)/2;
    geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * 4.0f)/2;
    geomInfo.position.z = 0;

    for(int i=0; i<4; i++){
         geomInfo.position += geomInfo.dx * 1.1f;
        for(int j=0; j < 4; j++){
                geomInfo.position += geomInfo.dy * 1.1f;
                scene->addChild(true, builder->createQuad(geomInfo, stateInfo));
        }
        geomInfo.position =  geomInfo.position  - (geomInfo.dy * 1.1f * 4.0f);
    }


 }


  void Creator::createRaw(RawGeometry raw){

        plProcessor.transform(raw);
        //raw.initPipeLine();
        scene->addChild(true, raw.transform);
  };

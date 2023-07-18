#include "Creator.h"
#include "TextCreator.h"



Creator::Creator(){

        builder = vsg::Builder::create();
      
    

};

void Creator::init(){

    std::cout << "CREATOR CREADO...." << std::endl;
};


void Creator::setScene(vsg::ref_ptr<vsg::Switch> in_scene){
     // POSICION EN EL MUNDO

        geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
        geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
        geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);

    scene = in_scene;
};


void Creator::resetPositions(){
     // POSICION EN EL MUNDO

        geomInfo.dx.set(1000000.0f, 0.0f, 0.0f);
        geomInfo.dy.set(0.0f, 1000000.0f, 0.0f);
        geomInfo.dz.set(0.0f, 0.0f, 1000000.0f);

    
};

void Creator::setOptions(vsg::ref_ptr<vsg::Options> in_options, vsg::ref_ptr<vsg::Font> in_font){
    local_options = in_options;
    font = in_font;

    builder->shaderSet = vsg::createPhongShaderSet(local_options);
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

 void Creator::createPlane(int n, int m){

    stateInfo.wireframe = false;
    stateInfo.instance_colors_vec4 = true;
    stateInfo.lighting = true;
 
 
    
    auto colors = vsg::vec4Array::create(1);

    colors->at(0).set(1.0f, 0.0f, 0.0f,1.0f);
    

    geomInfo.colors = colors;    
    
    geomInfo.position.x =geomInfo.position.x - (geomInfo.dx.x * 1.1f * n)/2;
    geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * m)/2;
    geomInfo.position.z = 0;

    for(int i=0; i<n; i++){
        geomInfo.position += geomInfo.dx * 1.1f;
        for(int j=0; j < m; j++){
                geomInfo.position += geomInfo.dy * 1.1f;
                scene->addChild(true, builder->createQuad(geomInfo, stateInfo));
        }
        geomInfo.position =  geomInfo.position  - (geomInfo.dy * (1.1f * m));
    }


    // Labels en X

    resetPositions();
    geomInfo.position.x =geomInfo.position.x - (geomInfo.dx.x * 1.1f * n);
    //geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * m);
    geomInfo.position.z = 0;

    for(int i=0; i<n; i++){
        geomInfo.position += geomInfo.dx * 1.1f;
        auto text = TextCreator::create(std::to_string(i), geomInfo.position.x, geomInfo.position.y, 100000, 500000, font, local_options);
        scene->addChild(true,text);
    }

    // Labels en Y
    resetPositions();
      geomInfo.position.x =geomInfo.position.x + (geomInfo.dx.x * 1.1f );
   // geomInfo.position.y = geomInfo.position.y - (geomInfo.dy.y * 1.1f * m);
    geomInfo.position.z = 0;

    for(int j=0; j<n; j++){
        geomInfo.position += geomInfo.dy * 1.1f;
        auto text = TextCreator::create(std::to_string(j), geomInfo.position.x, geomInfo.position.y, 100000, 500000, font, local_options);
        scene->addChild(true,text);
    }

   

 }




void Creator::createRadial(int n, int m){

    stateInfo.wireframe = true;

    geomInfo.position.x = 0;
    geomInfo.position.y = 0;
    geomInfo.position.z = 0;

   for(int i=0; i<n; i++){
        
        scene->addChild(true, builder->createDisk(geomInfo, stateInfo));
        geomInfo.dx += {1000000.0f, 0.0f, 0.0f};
        geomInfo.dy += {0.0f, 1000000.0f, 0.0f};
    }
    

 }


 void Creator::createRadialDepth(int n, int m){

    stateInfo.wireframe = true;

    geomInfo.position.x = 0;
    geomInfo.position.y = 0;
    geomInfo.position.z = 0;

   for(int i=1; i<n; i++){
        
        scene->addChild(true, builder->createDisk(geomInfo, stateInfo));
        geomInfo.dx += {1000000.0f, 0.0f, 0.0f};
        geomInfo.dy += {0.0f, 1000000.0f, 0.0f};
        geomInfo.position.z += (1000000 / (i )) ;
    }
    

 }


void Creator::createSpherical(int n, int m){

    stateInfo.wireframe = true;

    geomInfo.position.x = 0;
    geomInfo.position.y = 0;
    geomInfo.position.z = 0;

    geomInfo.position.x = geomInfo.position.x + (geomInfo.dx.x * 1.1f * n)/2;
    geomInfo.position.y = geomInfo.position.y + (geomInfo.dy.y * 1.1f * m)/2;
    geomInfo.position.z = 0;





   for(int i=0; i<n; i++){
        
        scene->addChild(true, builder->createSphere(geomInfo, stateInfo));
        geomInfo.dx += {1000000.0f, 0.0f, 0.0f};
        geomInfo.dy += {0.0f, 1000000.0f, 0.0f};
        geomInfo.dz += {0.0f, 0.0f, 1000000.0f};
    }
    

 }


void Creator::createCylindrical(int n, int m){

    stateInfo.wireframe = true;

    geomInfo.position.x = 0;
    geomInfo.position.y = 0;
    geomInfo.position.z = 0;

    geomInfo.position.x = geomInfo.position.x + (geomInfo.dx.x * 1.1f * n)/2;
    geomInfo.position.y = geomInfo.position.y + (geomInfo.dy.y * 1.1f * m)/2;
    geomInfo.position.z = 0;




   for(int i=0; i<n; i++){
        
        geomInfo.dx += {1000000.0f, 0.0f, 0.0f};
        geomInfo.dy += {0.0f, 1000000.0f, 0.0f};
        geomInfo.dz += {0.0f, 0.0f, 1000000.0f};
        scene->addChild(true, builder->createCylinder(geomInfo, stateInfo));
     
    }
    

 }













  void Creator::createRaw(RawGeometry raw){

        plProcessor.transform(raw);
        //raw.initPipeLine();
        scene->addChild(true, raw.transform);
  };

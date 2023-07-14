#include <vsg/all.h>





class EarthSceneCreator {

public:


static   vsg::ref_ptr<vsg::Group> create(vsg::ref_ptr<vsg::Options> options, vsg::ref_ptr<vsg::TileDatabaseSettings> settings){

          //  vsg::ref_ptr<vsg::TileDatabaseSettings> settings;
            // setup OpenStreetMap settings
          //  settings = vsg::TileDatabaseSettings::create();
            settings->extents = {{-180.0, -90.0, 0.0}, {180.0, 90.0, 1.0}};
            settings->noX = 1;
            settings->noY = 1;
            settings->maxLevel = 17;
            settings->originTopLeft = true;
            settings->lighting = false;
            settings->projection = "EPSG:3857"; // Spherical Mecator
            settings->imageLayer = "http://a.tile.openstreetmap.org/{z}/{x}/{y}.png";

            //arguments.read("-t", settings->lodTransitionScreenHeightRatio);
            // arguments.read("-m", settings->maxLevel);

            auto ellipsoidModel = settings->ellipsoidModel;
            
           


            auto earth = vsg::TileDatabase::create();
            earth->settings = settings;
            earth->readDatabase(options);

            /*---------ESCENA CARTOGRAFICA -----------*/
            
            // Se crea un Grupo conteniendo la BD geografica y otro nodo para contener
            // a los actores colocados sobre la cartografia (por ahora solo un BOX)


            auto earth_scene = vsg::Group::create();
            earth_scene->addChild(earth);

            return earth_scene;
    }

};
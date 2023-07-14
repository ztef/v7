#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>
#include <vsgImGui/Texture.h>
#include <vsgImGui/imgui.h>
#include <vsgImGui/implot.h>



#include <vsg/all.h>
#include "viI/all.h"




#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <iostream>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

/*-----------------------------------------------------

    VISUAL INTERACTION SYSTEMS

    viINavigator


------------------------------------------------------*/



int main(int argc, char **argv)
{

    /*----------------------------------------------------------------------


            ARGUMENTOS Y SET UP GENERAL

    ------------------------------------------------------------------------*/

    // Lector de argumentos via Linea de Comando
    vsg::CommandLine arguments(&argc, argv);

    // Carga vsg::Options para usar variables de ambienete, lectura de archivos etc
    auto options = vsg::Options::create();
    options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
    options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

    // Usa vsgXchange para lectura de archivos en diferentes formatos (autocad, obj etc)
    options->add(vsgXchange::all::create());

    arguments.read(options);

    // Ventana y Nombre de la ventana a desplegar
    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "viINavigator v0.0.1";



    auto window = vsg::Window::create(windowTraits);
    
    auto numFrames = -1;
    bool osgEarthStyleMouseButtons = true;
    
    uint32_t numOperationThreads = 0;
   

    // Usa setLineWidth para poder manejar el ancho desde un slider del GUI
    auto setLineWidth = vsg::SetLineWidth::create(1.0f);





    /*---------------------------------

        0 : CONTROLADOR DE LA ESCENA

        Realiza Switcheo entre escenas registradas
    
    -----------------------------------*/

    SceneController scenecontroller;




    /*-------------------------------------------------


        1:  CONSTRUCTORES DE OBJETOS


        --------------------------------------------------*/

        // 1.1  : Builder generico para construir geometrias primitivas en demanda :

            auto builder = vsg::Builder::create();
            vsg::StateInfo stateInfo;

            
            stateInfo.lighting = true;
        // 1.2 : CREA OBJETO CREATOR para crear objetos complejos  e insertarlos a la escena.

            Creator creator;


    /*----------------------------------------------------

        2:    NODOS (ESCENAS)

                Se inicializan 2 Escenas :

                    - Una Cartografica Base
                    - Una Abstracta Base

    ------------------------------------------------------*/



                            /*-------------------------------------------------


                                NODO CARTOGRAFICO

                            ---------------------------------------------------*/

                            /*

                            vsg::ref_ptr<vsg::TileDatabaseSettings> settings;
                            // setup OpenStreetMap settings
                            settings = vsg::TileDatabaseSettings::create();
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



                            
                            
                            // Se crea un Grupo conteniendo la BD geografica y otro nodo para contener
                            // a los actores colocados sobre la cartografia (por ahora solo un BOX)


                            auto earth_scene = vsg::Group::create();
                            earth_scene->addChild(earth);


                            */

                            //vsg::ref_ptr<vsg::TileDatabaseSettings> settings;           
                            auto settings = vsg::TileDatabaseSettings::create();

                            auto earth_scene = EarthSceneCreator::create(options,settings);
                            

                            auto ellipsoidModel = settings->ellipsoidModel;



                            vsg::GeometryInfo geomInfo;


                            //auto earth_actors = vsg::MatrixTransform::create();
                            //earth_actors->matrix = ellipsoidModel->computeLocalToWorldTransform({25.686613, -100.316116, 0.0});


                            //auto ecef = ellipsoidModel->convertLatLongAltitudeToECEF({25.686613, -100.316116, 0.0});
                            //auto ecef_normal = vsg::normalize(ecef);
                            //geomInfo.position = ecef;

                            geomInfo.dx.set(20000.0f, 0.0f, 0.0f);
                            geomInfo.dy.set(0.0f, 20000.0f, 0.0f);
                            geomInfo.dz.set(0.0f, 0.0f, 250000.0f);


                            

                            geomInfo.transform = ellipsoidModel->computeLocalToWorldTransform({25.686613, -100.316116, 0.0});

                            auto box = builder->createCylinder(geomInfo, stateInfo);
                            box->setValue("name", "CAJA");


                            //earth_actors->addChild(box);

                            earth_scene->addChild(box);


                          
                            /* segundo cilindro


                            geomInfo.dx.set(200000.0f, 0.0f, 0.0f);
                            geomInfo.dy.set(0.0f, 200000.0f, 0.0f);
                            geomInfo.dz.set(0.0f, 0.0f, 250000.0f);

                            
                            vsg::vec3 cylinderAxis(0.0f, 1.0f, 0.0f); 

                            auto  rot = vsg::rotate(vsg::radians(45.0f), 0.0f, 1.0f, 0.0f);
                            //auto scale = vsg::scale(vsg::vec3(1.0f, 2.0f, 3.0f));
                            //vsg::mat4 nada;
                            geomInfo.transform = rot;
                            


                            auto box1 = builder->createCylinder(geomInfo, stateInfo);
                            box1->setValue("name", "CAJA1");
                            earth_scene->addChild(box1);

                            */



                            // Agrega LUZ

                            auto directionalLight1 = vsg::DirectionalLight::create();
                            directionalLight1->name = "directional";
                            directionalLight1->color.set(1.0, 1.0, 1.0);
                            directionalLight1->intensity = 0.99;
                            directionalLight1->direction.set(0.0, -1.0, -1.0);
                            earth_scene->addChild( directionalLight1);



                            /*
                            const double invalid_value = std::numeric_limits<double>::max();
                            double poi_latitude = invalid_value;
                            double poi_longitude = invalid_value;
                            double poi_distance = invalid_value;

                            while (arguments.read("--poi", poi_latitude, poi_longitude))
                            {
                            };
                            while (arguments.read("--distance", poi_distance))
                            {
                            };

                            if (arguments.errors())
                                return arguments.writeErrorMessages(std::cerr);

                            if (!outputFilename.empty())
                            {
                                vsg::write(earth_scene, outputFilename);
                                return 0;
                            }
                            */


                            /*-------------------------------------------------


                                NODO ABSTRACTO

                            ---------------------------------------------------*/

                            //  Escena o Nodo SWITCH. Contiene sub escenas/nodos switcheables

                            /*----------vsg SCENE --------------------*/
                            auto abstract_scene = vsg::Switch::create();

                           

                            // REGISTRA LA ESCENA EN EL CREATOR
                            creator.setScene(abstract_scene);
                            creator.setOptions(options);


                                        auto directionalLight = vsg::DirectionalLight::create();
                                        directionalLight->name = "directional";
                                        directionalLight->color.set(1.0, 1.0, 1.0);
                                        directionalLight->intensity = 0.99;
                                        directionalLight->direction.set(0.0, -1.0, -1.0);
                                        abstract_scene->addChild(true, directionalLight);



                                        auto abstract_nodes = vsg::Switch::create();



                                        /*--------------- XYZ SCENE -----------------*/

                                        auto xyz_scene = vsg::Switch::create();
                                        creator.setScene(xyz_scene);
                                        creator.createPlane(20,20); // Crea un plano
                                        //creator.createPrimitive("box");

                                        /*  Agregada a escena XYZ */
                                        abstract_nodes->addChild(true, xyz_scene);


                                         



                                          /*--------------- radial SCENE -----------------*/

                                        auto radial_scene = vsg::Switch::create();
                                        creator.setScene(radial_scene);
                                        creator.createRadial(20,20); // Crea un radial
                                        

                                        /*  Agregada a escena XYZ */
                                        abstract_nodes->addChild(false, radial_scene);


                                         /*--------------- radial Depth SCENE -----------------*/

                                        auto radiald_scene = vsg::Switch::create();
                                        creator.setScene(radiald_scene);
                                        creator.createRadialDepth(20,20); // Crea un radial
                                        

                                        /*  Agregada a escena XYZ */
                                        abstract_nodes->addChild(false, radiald_scene);



                                        /*--------------- spherical SCENE -----------------*/

                                        auto spherical_scene = vsg::Switch::create();
                                        creator.setScene(spherical_scene);
                                        creator.createSpherical(1,1); // Crea un Sphere
                                        

                                        /*  Agregada a escena XYZ */
                                        abstract_nodes->addChild(false, spherical_scene);


                                         /*--------------- cylindrical SCENE -----------------*/

                                        auto cylindrical_scene = vsg::Switch::create();
                                        creator.setScene(cylindrical_scene);
                                        creator.createCylindrical(1,1); // Crea un Sphere
                                        

                                        /*  Agregada a escena XYZ */
                                        abstract_nodes->addChild(false, cylindrical_scene);


                            /* AGREGA TODOS A ABSTRACT_SCENE */

                            abstract_scene->addChild(true, abstract_nodes);


                            // NODO TESTING :

                                /*
                                stateInfo.wireframe = false;
                                stateInfo.instance_colors_vec4 = true;
                                stateInfo.lighting = true;
                                builder->shaderSet = vsg::createPhongShaderSet(options);

                                auto colors = vsg::vec4Array::create(1);
                                            geomInfo.colors = colors;
                                            for (auto& c : *(colors))
                                            {
                                                c.set(float(std::rand()) / float(RAND_MAX), float(std::rand()) / float(RAND_MAX), float(std::rand()) / float(RAND_MAX), 1.0f);
                                            }




                                    abstract_scene->addChild(true, builder->createCapsule(geomInfo, stateInfo));            
                                    */




    /*----------------------------------------------------------------------

            ESCENA, Elementos de interaccion  GUI

    -----------------------------------------------------------------------*/

    // PREPARA LISTENER (Teclado)

    InputKeyboardValues keyb;



    // GUI,  CREA UN NODO ImGui

    auto params = Params::create();
    // auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options));
    auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options, setLineWidth, &keyb, &scenecontroller));

  
  
  
    /*------------------------------------------------------------------------

        VIEWER

    -------------------------------------------------------------------------*/

    auto viewer = vsg::Viewer::create();

    if (!window)
    {
        std::cout << "Could not create windows." << std::endl;
        return 1;
    }

    viewer->addWindow(window);

    uint32_t width = window->extent2D().width;
    uint32_t height = window->extent2D().height;




    /*--------------------------------------------------------------------------


        CAMARAS 

                Crea las camaras y para cada una una Vista Asociada

    -----------------------------------------------------------------------------*/



            // CAMARA GEOGRAFICA (Elipsoide)


                /*
                // Bounds y Parametros 

                vsg::ComputeBounds computeBounds;
                earth_scene->accept(computeBounds);

                vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
                double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.9;
                double nearFarRatio = 0.001;

                
                vsg::ref_ptr<vsg::LookAt> lookAt;
                vsg::ref_ptr<vsg::ProjectionMatrix> perspective;

                lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

                perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, horizonMountainHeight);

                //  Crea Camara :

                //auto camera_geo = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));
                
                
                */


                auto camera_geo = createCameraForGeoScene(earth_scene, 0, 0, width, height, window, ellipsoidModel);


                // Crea VISTA

                auto earth_view = vsg::View::create(camera_geo, earth_scene);

    


            // CAMARA PARA ESCENAS ABSTRACTAS

      
               
   
                // Camara 
                auto camera_abstract = createCameraForScene(abstract_scene, 0, 0, width, height);

                // Vista
                auto abstract_view = vsg::View::create(camera_abstract, abstract_scene);

   


    /*-----------------------------------------------------


        MANEJO DE EVENTOS 


    --------------------------------------------------------*/

   
   
            // ESCAPE Y Cerrado de ventana
            viewer->addEventHandler(vsg::CloseHandler::create(viewer));

            //  Registramos como Listener al GUI (El primero en registrarse recibe primero los eventos)
            viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());

   

            // Eventos de Navegacion en la geografia :

             //25.686613 -100.316116

            auto trackball = vsg::Trackball::create(camera_geo, ellipsoidModel);
            trackball->addKeyViewpoint(vsg::KeySymbol('1'), 25.686613, -100.316116, 2000.0, 2.0); // Monterrey
            trackball->addKeyViewpoint(vsg::KeySymbol('2'), 55.948642740309324, -3.199226855522667, 2000.0, 2.0);  // Edinburgh Castle
            trackball->addKeyViewpoint(vsg::KeySymbol('3'), 48.858264952330764, 2.2945039609604665, 2000.0, 2.0);  // Eiffel Town, Paris
            trackball->addKeyViewpoint(vsg::KeySymbol('4'), 52.5162603714634, 13.377684902745642, 2000.0, 2.0);    // Brandenburg Gate, Berlin
            trackball->addKeyViewpoint(vsg::KeySymbol('5'), 30.047448591298807, 31.236319571791213, 10000.0, 2.0); // Cairo
            trackball->addKeyViewpoint(vsg::KeySymbol('6'), 35.653099536061156, 139.74704060056993, 10000.0, 2.0); // Tokyo
            trackball->addKeyViewpoint(vsg::KeySymbol('7'), 37.38701052699002, -122.08555895549424, 10000.0, 2.0); // Mountain View, California
            trackball->addKeyViewpoint(vsg::KeySymbol('8'), 40.689618207006355, -74.04465595488215, 10000.0, 2.0); // Empire State Building
            trackball->addKeyViewpoint(vsg::KeySymbol('9'), 25.997055873649554, -97.15543476551771, 1000.0, 2.0);  // Boca Chica, Taxas

          
            // Trackball para escenas abstractas
            auto tb1 = vsg::Trackball::create(camera_abstract);





            // Crea un MOUSEHANDLER para manejar los 2 trackballs. Uno para la escena geografica y otro para la abstracta.
            // La razon de ello es que la escena geografica usa un modelo Elipsoide.

            auto mouseHandler = MouseHandler::create(trackball, tb1);
           

            // Solo registra el mouseHandler. Este rutea los eventos del mouse y teclado al trackball que este activo
            viewer->addEventHandler(mouseHandler);

   
   
    /*--------------------------------------------------------


        ARMADO DE RenderGraph, CommandGraph

    ----------------------------------------------------------*/


    // ViewSelector es un Switch que integra vistas (geografica y abstracta)

    auto viewTypeSelector = vsg::Switch::create();

    // le agrega las vistas a manejar :
    viewTypeSelector->addChild(false, earth_view);
    viewTypeSelector->addChild(true, abstract_view);

    // SCENE CONTROLER Debe recibir tanto el Switch, como el mouse Handler
    scenecontroller.setSwitchNode(viewTypeSelector);
    scenecontroller.setMouseHandler(mouseHandler);

    scenecontroller.setSwitchAbstract(abstract_nodes);


    auto renderGraph = vsg::RenderGraph::create(window);

    // add main view that covers the whole window.
    renderGraph->addChild(viewTypeSelector);
    renderGraph->addChild(renderImGui);

    // clear the depth buffer before view2 gets rendered
    /*
    VkClearValue colorClearValue{};
    colorClearValue.color = {{0.2f, 0.2f, 0.2f, 1.0f}};
    VkClearAttachment color_attachment{VK_IMAGE_ASPECT_COLOR_BIT, 0, colorClearValue};

    VkClearValue depthClearValue{};
    depthClearValue.depthStencil = {0.0f, 0};
    VkClearAttachment depth_attachment{VK_IMAGE_ASPECT_DEPTH_BIT, 1, depthClearValue};

    VkClearRect rect{camera_abstract->getRenderArea(), 0, 1};
    auto clearAttachments = vsg::ClearAttachments::create(vsg::ClearAttachments::Attachments{color_attachment, depth_attachment}, vsg::ClearAttachments::Rects{rect, rect});
    */


    // renderGraph->addChild(clearAttachments);

    // add the second insert view that overlays ontop.
    // renderGraph->addChild(abstract_view);



    // option 1
    auto commandGraph = vsg::CommandGraph::create(window);
    commandGraph->addChild(renderGraph);

    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});


    
    // option 2
    //auto commandGraph = vsg::createCommandGraphForView(window, camera_abstract, abstract_scene);
    //viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});





    // add the view handler for interactively changing the views
    //viewer->addEventHandler(ViewHandler::create(renderGraph));

    // Creamos un IntersectionHandler que va a monitorear intersecciones para picking de objetos
    // Por ahora recibe un builder para poder generar objetos dinammicos en la escena
    auto intersectionHandler = IntersectionHandler::create(builder, camera_abstract, xyz_scene, ellipsoidModel, 1000000.0f, options);
    intersectionHandler->state = stateInfo;

    // Registramos el Intersection Handler, tiene varios apply
     viewer->addEventHandler(intersectionHandler);

    builder->assignCompileTraversal(vsg::CompileTraversal::create(*viewer));

    viewer->compile();

    /*-------------------------------------------------------------------


        LOOP PRINCIPAL


    ---------------------------------------------------------------------*/

    while (viewer->advanceToNextFrame() && (numFrames < 0 || (numFrames--) > 0))
    {
        // pass any events into EventHandlers assigned to the Viewer
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    return 0;
}
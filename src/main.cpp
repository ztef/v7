#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>
#include <vsgImGui/Texture.h>
#include <vsgImGui/imgui.h>
#include <vsgImGui/implot.h>
#include "Interaction/Handlers/InputHandler.h"
#include "Interaction/Handlers/IntersectionHandler.h"
#include "Interaction/Handlers/SceneController.h"
#include "Creators/Creator.h"
#include "Menu/Main_menu.h"
#include "Creators/RawTestGeom.h"

#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#    include <vsgXchange/all.h>
#endif

#include <iostream>

 
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char** argv)
{


    /*----------------------------------------------------------------------


            ARGUMENTOS Y SET UP GENERAL

    ------------------------------------------------------------------------*/



    // set up defaults and read command line arguments to override them
    vsg::CommandLine arguments(&argc, argv);

    // set up vsg::Options to pass in filepaths and ReaderWriter's and other IO related options to use when reading and writing files.
    auto options = vsg::Options::create();
    options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
    options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

    // add vsgXchange's support for reading and writing 3rd party file formats
    options->add(vsgXchange::all::create());

    arguments.read(options);

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "viExplorer";
    windowTraits->debugLayer = arguments.read({"--debug", "-d"});
    windowTraits->apiDumpLayer = arguments.read({"--api", "-a"});

    if (arguments.read("--IMMEDIATE")) windowTraits->swapchainPreferences.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    if (arguments.read({"--fullscreen", "--fs"})) windowTraits->fullscreen = true;
    if (arguments.read({"--window", "-w"}, windowTraits->width, windowTraits->height)) { windowTraits->fullscreen = false; }

    arguments.read("--screen", windowTraits->screenNum);
    arguments.read("--display", windowTraits->display);
    arguments.read("--samples", windowTraits->samples);

    auto outputFilename = arguments.value(std::string(), "-o");
    auto numFrames = arguments.value(-1, "-f");
    auto pathFilename = arguments.value(std::string(), "-p");
    auto maxPagedLOD = arguments.value(0, "--maxPagedLOD");
    auto loadLevels = arguments.value(0, "--load-levels");
    auto horizonMountainHeight = arguments.value(0.0, "--hmh");
    bool useEllipsoidPerspective = !arguments.read({"--disble-EllipsoidPerspective", "--dep"});
    
    if (arguments.read("--rgb")) options->mapRGBtoRGBAHint = false;
    arguments.read("--file-cache", options->fileCache);
    bool osgEarthStyleMouseButtons = arguments.read({"--osgearth", "-e"});

    VkClearColorValue clearColor{{0.2f, 0.2f, 0.4f, 1.0f}};
    arguments.read({"--bc", "--background-color"}, clearColor.float32[0], clearColor.float32[1], clearColor.float32[2], clearColor.float32[3]);

    uint32_t numOperationThreads = 0;
    if (arguments.read("--ot", numOperationThreads)) options->operationThreads = vsg::OperationThreads::create(numOperationThreads);

   


      // Usa setLineWidth para poder manejar el ancho desde un slider del GUI
        auto setLineWidth = vsg::SetLineWidth::create(1.0f);
        SceneController scenecontroller;


   
    /*----------------------------------------------------
    
            CARTOGRAFIA

    ------------------------------------------------------*/
   
   
    vsg::ref_ptr<vsg::TileDatabaseSettings> settings;
     // setup OpenStreetMap settings
        settings = vsg::TileDatabaseSettings::create();
        settings->extents = {{-180.0, -90.0, 0.0}, {180.0, 90.0, 1.0}};
        settings->noX = 1;
        settings->noY = 1;
        settings->maxLevel = 17;
        settings->originTopLeft = true;
        settings->lighting = false;
        settings->projection = "EPSG:3857";  // Spherical Mecator
        settings->imageLayer = "http://a.tile.openstreetmap.org/{z}/{x}/{y}.png";

        arguments.read("-t", settings->lodTransitionScreenHeightRatio);
        arguments.read("-m", settings->maxLevel);

        auto ellipsoidModel = settings->ellipsoidModel;

        auto earth = vsg::TileDatabase::create();
        earth->settings = settings;
        earth->readDatabase(options);

        
        /*---------EARTH SCENE -----------*/
        auto earth_scene = earth;

        const double invalid_value = std::numeric_limits<double>::max();
        double poi_latitude = invalid_value;
        double poi_longitude = invalid_value;
        double poi_distance = invalid_value;
        
        while (arguments.read("--poi", poi_latitude, poi_longitude)) {};
        while (arguments.read("--distance", poi_distance)) {};

        if (arguments.errors()) return arguments.writeErrorMessages(std::cerr);

        if (!outputFilename.empty())
        {
            vsg::write(earth_scene, outputFilename);
            return 0;
        }




    /*------------------------------------------------------------------------
    

        VIEWER

    -------------------------------------------------------------------------*/
    
                auto viewer = vsg::Viewer::create();
                auto window = vsg::Window::create(windowTraits);
                if (!window)
                {
                    std::cout << "Could not create windows." << std::endl;
                    return 1;
                }

                viewer->addWindow(window);



     /*-------------------------------------------------
     
     
                1:  CONSTRUCTORES DE OBJETOS 

     
     --------------------------------------------------*/
    
    
    
                    // 1.1  : Builder generico para construir objetos en demanda :

                    auto builder = vsg::Builder::create();
                    vsg::StateInfo stateInfo;


                     // 1.2 : CREA OBJETO CREATOR para crear objetos e insertarlos a la escena.

                    Creator creator;




  //  Escena o Nodo SWITCH. Contiene sub escenas/nodos switcheables 
                
       
                    /*----------vsg SCENE --------------------*/
                    auto vsg_scene = vsg::Switch::create();

                    scenecontroller.setSwitchNode(vsg_scene);


                    // REGISTRA LA ESCENA EN EL CREATOR
                    creator.setScene(vsg_scene);
                    creator.setOptions(options);



/*----------------------------------------------------------------------

        ESCENA, Elementos de interaccion  GUI

    -----------------------------------------------------------------------*/




                // PREPARA LISTENER (Teclado)

                InputKeyboardValues keyb;




                // GUI,  CREA UN NODO ImGui 


                auto params = Params::create();
                //auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options));
                 auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(setLineWidth, &keyb, &scenecontroller));



/*------------------------------------------------------------------------


    ESCENAS


----------------------------------------------------------------------------*/



                /*--------------- XYZ SCENE -----------------*/

                auto xyz_scene = vsg::Switch::create();
                creator.setScene(xyz_scene);
                creator.createPlane();


                /*  Agregada a escena XYZ */
                vsg_scene->addChild(true,xyz_scene);
                
                vsg_scene->addChild(true, earth_scene);

    /*--------------------------------------------------------------------------


        BOUNDS Y CAMARA PARA LA ESCENA GEOGRAFICA

    -----------------------------------------------------------------------------*/
   
                vsg::ComputeBounds computeBounds;
                vsg_scene->accept(computeBounds);


                vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
                double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.9;
                double nearFarRatio = 0.001;

                // set up the camera
                vsg::ref_ptr<vsg::LookAt> lookAt;
                vsg::ref_ptr<vsg::ProjectionMatrix> perspective;


                // Si hay puntos de interes :

                if (poi_latitude != invalid_value && poi_longitude != invalid_value)
                {
                    double height = (poi_distance != invalid_value) ? poi_distance : radius * 3.5;
                    auto ecef = ellipsoidModel->convertLatLongAltitudeToECEF({poi_latitude, poi_longitude, 0.0});
                    auto ecef_normal = vsg::normalize(ecef);

                    centre = ecef;
                    vsg::dvec3 eye = centre + ecef_normal * height;
                    vsg::dvec3 up = vsg::normalize(vsg::cross(ecef_normal, vsg::cross(vsg::dvec3(0.0, 0.0, 1.0), ecef_normal)));

                    // set up the camera
                    lookAt = vsg::LookAt::create(eye, centre, up);
                }
                else
                {
                    lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));
                }


                // modo elipsoide :
                if (useEllipsoidPerspective)
                {
                    perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, horizonMountainHeight);
                }
                else
                {
                    perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio * radius, radius * 4.5);
                }


                //  Crea Camara :

                auto camera_geo = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));





/*--------------------------------------------------------------------------


        BOUNDS Y CAMARA PARA ESCENAS ABSTRACTAS

    -----------------------------------------------------------------------------*/
        
                vsg::ComputeBounds computeBounds1;
                vsg_scene->accept(computeBounds1);


                vsg::dvec3 centre1 = (computeBounds1.bounds.min + computeBounds1.bounds.max) * 0.5;
                double radius1 = vsg::length(computeBounds1.bounds.max - computeBounds1.bounds.min) * 0.9;
                double nearFarRatio1 = 0.001;

                // set up the camera
               // vsg::ref_ptr<vsg::LookAt> lookAt1;
               // vsg::ref_ptr<vsg::ProjectionMatrix> perspective1;


               
                
                auto perspective1 = vsg::Perspective::create(90.0, 1.0, nearFarRatio1 * radius1, radius1 * 4.0);
                auto lookAt1 = vsg::LookAt::create(centre1 + vsg::dvec3(0.0, 0.0, radius1 * 2.0), centre1, vsg::dvec3(0.0, 1.0, 0.0));
                auto camera_xyz = vsg::Camera::create(perspective1, lookAt1);

               
               // viewer->addEventHandler(vsg::Trackball::create(camera_xyz));



/*-----------------------------------------------------


    Event Handlers

--------------------------------------------------------*/


    // ESCAPE Y Cerrado de ventana
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    //  Registramos como Listener al GUI (El primero en registrarse recibe primero los eventos)
    viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());

    
    
    // Eventos de Navegacion en la geografia :


                    if (pathFilename.empty())
                    {
                        if (ellipsoidModel)
                        {
                            auto trackball = vsg::Trackball::create(camera_geo, ellipsoidModel);
                            trackball->addKeyViewpoint(vsg::KeySymbol('1'), 51.50151088842245, -0.14181489107549874, 2000.0, 2.0); // Grenwish Observatory
                            trackball->addKeyViewpoint(vsg::KeySymbol('2'), 55.948642740309324, -3.199226855522667, 2000.0, 2.0);  // Edinburgh Castle
                            trackball->addKeyViewpoint(vsg::KeySymbol('3'), 48.858264952330764, 2.2945039609604665, 2000.0, 2.0);  // Eiffel Town, Paris
                            trackball->addKeyViewpoint(vsg::KeySymbol('4'), 52.5162603714634, 13.377684902745642, 2000.0, 2.0);    // Brandenburg Gate, Berlin
                            trackball->addKeyViewpoint(vsg::KeySymbol('5'), 30.047448591298807, 31.236319571791213, 10000.0, 2.0); // Cairo
                            trackball->addKeyViewpoint(vsg::KeySymbol('6'), 35.653099536061156, 139.74704060056993, 10000.0, 2.0); // Tokyo
                            trackball->addKeyViewpoint(vsg::KeySymbol('7'), 37.38701052699002, -122.08555895549424, 10000.0, 2.0); // Mountain View, California
                            trackball->addKeyViewpoint(vsg::KeySymbol('8'), 40.689618207006355, -74.04465595488215, 10000.0, 2.0); // Empire State Building
                            trackball->addKeyViewpoint(vsg::KeySymbol('9'), 25.997055873649554, -97.15543476551771, 1000.0, 2.0);  // Boca Chica, Taxas

                            if (osgEarthStyleMouseButtons)
                            {
                                trackball->panButtonMask = vsg::BUTTON_MASK_1;
                                trackball->rotateButtonMask = vsg::BUTTON_MASK_2;
                                trackball->zoomButtonMask = vsg::BUTTON_MASK_3;
                            }

                            viewer->addEventHandler(trackball);
                        }
                        else
                        {
                            viewer->addEventHandler(vsg::Trackball::create(camera_geo));
                        }
                    }
                    else
                    {
                        auto animationPath = vsg::read_cast<vsg::AnimationPath>(pathFilename, options);
                        if (!animationPath)
                        {
                            std::cout<<"Warning: unable to read animation path : "<<pathFilename<<std::endl;
                            return 1;
                        }
                        viewer->addEventHandler(vsg::AnimationPathHandler::create(camera_geo, animationPath, viewer->start_point()));
                    }

                    // if required pre load specific number of PagedLOD levels.
                    /*
                    if (loadLevels > 0)
                    {
                        vsg::LoadPagedLOD loadPagedLOD(camera, loadLevels);

                        auto startTime = std::chrono::steady_clock::now();

                        earth_scene->accept(loadPagedLOD);

                        auto time = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::steady_clock::now() - startTime).count();
                        std::cout << "No. of tiles loaed " << loadPagedLOD.numTiles << " in " << time << "ms." << std::endl;
                    }
                    */


    /*--------------------------------------------------------
    

        ARMADO DE RenderGraph, CommandGraph

    ----------------------------------------------------------*/



    /*------ AGREGA LA ESCENA GEOGRAFICA A LA ESCENA vsg -----------*/
    //vsg_scene->addChild(true, earth_scene);



    auto rendergraph = vsg::createRenderGraphForView(window, camera_geo, vsg_scene);
    rendergraph->setClearValues(clearColor);


    rendergraph->addChild(renderImGui);

    auto commandGraph = vsg::CommandGraph::create(window, rendergraph);
    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

    viewer->compile();


            if (maxPagedLOD > 0)
            {
                // set targetMaxNumPagedLODWithHighResSubgraphs after Viewer::compile() as it will assign any DatabasePager if required.
                for(auto& task : viewer->recordAndSubmitTasks)
                {
                    if (task->databasePager) task->databasePager->targetMaxNumPagedLODWithHighResSubgraphs = maxPagedLOD;
                }
            }

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
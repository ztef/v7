#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>
#include <vsgImGui/Texture.h>
#include <vsgImGui/imgui.h>
#include <vsgImGui/implot.h>
#include "Interaction/Handlers/InputHandler.h"
#include "Interaction/Handlers/IntersectionHandler.h"
#include "Interaction/Handlers/SceneController.h"
#include "Interaction/Handlers/MouseHandler.h"
#include "Interaction/Handlers/CameraTools.h"
#include "Creators/Creator.h"
#include "Menu/Main_menu.h"
#include "Creators/RawTestGeom.h"

#include <vsg/all.h>

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

    viBNavigator


------------------------------------------------------*/



int main(int argc, char **argv)
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

    auto window = vsg::Window::create(windowTraits);

    if (arguments.read("--IMMEDIATE"))
        windowTraits->swapchainPreferences.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    if (arguments.read({"--fullscreen", "--fs"}))
        windowTraits->fullscreen = true;
    if (arguments.read({"--window", "-w"}, windowTraits->width, windowTraits->height))
    {
        windowTraits->fullscreen = false;
    }

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

    if (arguments.read("--rgb"))
        options->mapRGBtoRGBAHint = false;
    arguments.read("--file-cache", options->fileCache);
    bool osgEarthStyleMouseButtons = arguments.read({"--osgearth", "-e"});

    VkClearColorValue clearColor{{0.2f, 0.2f, 0.4f, 1.0f}};
    arguments.read({"--bc", "--background-color"}, clearColor.float32[0], clearColor.float32[1], clearColor.float32[2], clearColor.float32[3]);

    uint32_t numOperationThreads = 0;
    if (arguments.read("--ot", numOperationThreads))
        options->operationThreads = vsg::OperationThreads::create(numOperationThreads);

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

                            arguments.read("-t", settings->lodTransitionScreenHeightRatio);
                            arguments.read("-m", settings->maxLevel);

                            auto ellipsoidModel = settings->ellipsoidModel;

                            auto earth = vsg::TileDatabase::create();
                            earth->settings = settings;
                            earth->readDatabase(options);



                            /*---------ESCENA CARTOGRAFICA -----------*/
                            
                            // Se crea un Grupo conteniendo la BD geografica y otro nodo para contener
                            // a los actores colocados sobre la cartografia (por ahora solo un BOX)


                            auto earth_scene = vsg::Group::create();
                            earth_scene->addChild(earth);

                            vsg::GeometryInfo geomInfo;

                            geomInfo.dx.set(10000000.0f, 0.0f, 0.0f);
                            geomInfo.dy.set(0.0f, 10000000.0f, 0.0f);
                            geomInfo.dz.set(0.0f, 0.0f, 10000000.0f);

                            auto box = builder->createBox(geomInfo, stateInfo);
                            box->setValue("name", "CAJA");
                            earth_scene->addChild(box);

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

                            /*-------------------------------------------------


                                NODO ABSTRACTO

                            ---------------------------------------------------*/

                            //  Escena o Nodo SWITCH. Contiene sub escenas/nodos switcheables

                            /*----------vsg SCENE --------------------*/
                            auto abstract_scene = vsg::Switch::create();

                           

                            // REGISTRA LA ESCENA EN EL CREATOR
                            creator.setScene(abstract_scene);
                            creator.setOptions(options);

                            /*--------------- XYZ SCENE -----------------*/

                            auto xyz_scene = vsg::Switch::create();
                            creator.setScene(xyz_scene);
                            creator.createPlane(); // Crea un plano
                            creator.createPrimitive("box");

                            /*  Agregada a escena XYZ */
                            abstract_scene->addChild(true, xyz_scene);

   


   

    /*----------------------------------------------------------------------

            ESCENA, Elementos de interaccion  GUI

    -----------------------------------------------------------------------*/

    // PREPARA LISTENER (Teclado)

    InputKeyboardValues keyb;

    // GUI,  CREA UN NODO ImGui

    auto params = Params::create();
    // auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options));
    auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(setLineWidth, &keyb, &scenecontroller));

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


        CAMARA GEOGRAFICA

    -----------------------------------------------------------------------------*/

    vsg::ComputeBounds computeBounds;
    earth_scene->accept(computeBounds);

    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.9;
    double nearFarRatio = 0.001;

    // set up the camera
    vsg::ref_ptr<vsg::LookAt> lookAt;
    vsg::ref_ptr<vsg::ProjectionMatrix> perspective;

    lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

    perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, horizonMountainHeight);

    //  Crea Camara :

    auto camera_geo = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));
    // auto camera_geo = createCameraForScene(earth_scene, (width * 3) / 4, 0, width / 4, height / 4);
    auto earth_view = vsg::View::create(camera_geo, earth_scene);

    /*--------------------------------------------------------------------------


            BOUNDS Y CAMARA PARA ESCENAS ABSTRACTAS

        -----------------------------------------------------------------------------*/

    vsg::ComputeBounds computeBounds1;
    abstract_scene->accept(computeBounds1);

    vsg::dvec3 centre1 = (computeBounds1.bounds.min + computeBounds1.bounds.max) * 0.5;
    double radius1 = vsg::length(computeBounds1.bounds.max - computeBounds1.bounds.min) * 0.9;
    double nearFarRatio1 = 0.001;

    // set up the camera
    // vsg::ref_ptr<vsg::LookAt> lookAt1;
    // vsg::ref_ptr<vsg::ProjectionMatrix> perspective1;

    // auto camera_abstract = createCameraForScene(abstract_scene,  (width * 3) / 4, 0, width / 4, height / 4);

    auto camera_abstract = createCameraForScene(abstract_scene, 0, 0, width, height);
    auto abstract_view = vsg::View::create(camera_abstract, abstract_scene);

    //  viewer->addEventHandler(vsg::Trackball::create(camera_xyz));

    /*-----------------------------------------------------


        Event Handlers

    --------------------------------------------------------*/

    // ESCAPE Y Cerrado de ventana
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    //  Registramos como Listener al GUI (El primero en registrarse recibe primero los eventos)
    viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());

   

    // Eventos de Navegacion en la geografia :

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

    // viewer->addEventHandler(trackball);

    auto tb1 = vsg::Trackball::create(camera_abstract);



    // Crea un MOUSEHANDLER para manejar 2 trackballs. Uno para la escena geografica y otro para la abstracta.
    // La razon de ello es que la escena geografica usa un modelo Elipsoide.


    auto mouseHandler = MouseHandler::create(trackball, tb1);
    //hh->trackball_active = 1;

    viewer->addEventHandler(mouseHandler);

    /*--------------------------------------------------------


        ARMADO DE RenderGraph, CommandGraph

    ----------------------------------------------------------*/


    // ViewSelector es un Switch que integra vistas (geografica y abstracta)

    auto viewselector = vsg::Switch::create();

    // le agrega las vistas a manejar :
    viewselector->addChild(true, earth_view);
    viewselector->addChild(true, abstract_view);

    // SCENE CONTROLER Debe recibir tanto el Switch, como el mouse Handler
    scenecontroller.setSwitchNode(viewselector);
    scenecontroller.setMouseHandler(mouseHandler);

    auto renderGraph = vsg::RenderGraph::create(window);

    // add main view that covers the whole window.
    renderGraph->addChild(viewselector);
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

    auto commandGraph = vsg::CommandGraph::create(window);
    commandGraph->addChild(renderGraph);

    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

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
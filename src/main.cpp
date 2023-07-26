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



    // Tipo de letra
     auto font_filename = std::string("./fonts/times.vsgb");
     auto font = vsg::read_cast<vsg::Font>(font_filename, options);


    // Usa setLineWidth para poder manejar el ancho desde un slider del GUI
    auto setLineWidth = vsg::SetLineWidth::create(1.0f);


      auto builder = vsg::Builder::create();
     vsg::StateInfo stateInfo;

     stateInfo.lighting = true;



    /*
    
        0: Registry 

    */

   Registry registry;
   registry.builder = builder;
   



    /*---------------------------------

        0 : CONTROLADOR DE LA ESCENA

        Realiza Switcheo entre escenas registradas

    -----------------------------------*/

    SceneController scenecontroller;

        registry.scenecontroller = &scenecontroller;            // Registra


 /*---------------------------------

        1 : PROCESADOR DE COMANDOS

    -----------------------------------*/

    CommandProcessor commandProcessor;

    commandProcessor.registry = &registry;



    /*
        MOTOR DE TRANSFORMACIONES
    */

    TransformEngine transformEngine;


    /*-------------------------------------------------


        1:  CONSTRUCTORES DE OBJETOS


        --------------------------------------------------*/

    // 1.1  : Builder generico para construir geometrias primitivas en demanda :

  
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

                // ESCENA : earth_scene
                auto settings = vsg::TileDatabaseSettings::create();
                auto earth_scene = EarthSceneCreator::create(options, settings);

                auto ellipsoidModel = settings->ellipsoidModel;

                auto barra = GeomActorCreator::create(ellipsoidModel, builder, stateInfo, 25.686613, -100.316116);
                earth_scene->addChild(barra);

                auto barra1 = GeomActorCreator::create(ellipsoidModel, builder, stateInfo, 26.686613, -100.316116);
                earth_scene->addChild(barra1);

                auto luz = LightCreator::create();
                earth_scene->addChild(luz);


                registry.earth_scene = earth_scene;         // Registra

    /*-------------------------------------------------


        NODO ABSTRACTO

    ---------------------------------------------------*/

                /*----------ESCENA ABSTRACTA  --------------------*/
                auto abstract_scene = vsg::Switch::create();

                // REGISTRA LA ESCENA EN EL CREATOR
                creator.setScene(abstract_scene);
                creator.setOptions(options, font);

                auto luz1 = LightCreator::create();
                abstract_scene->addChild(true, luz1);

                auto abstract_nodes = vsg::Switch::create();


                registry.abstract_nodes = abstract_nodes;       // Registra

    /*--------------- XYZ SCENE -----------------*/

                        auto xyz_scene = vsg::Switch::create();
                        creator.setScene(xyz_scene);
                        creator.createPlane(30, 30); // Crea un plano

                        auto a =  ActorCreator::create(builder,stateInfo, 0,0,"DYNAMIC");
                        
                        
                        xyz_scene->addChild(true,a->node);
                        
                        registry.xyz_scene = xyz_scene;         // Registra

                        
                        abstract_nodes->addChild(true, xyz_scene);



    /*--------------- radial SCENE -----------------*/

                        auto radial_scene = vsg::Switch::create();
                        creator.setScene(radial_scene);
                        creator.createRadial(3,10); // Crea un radial

                        // RawTestGeom raw(options);

                       
                                     // Usa setLineWidth para poder manejar el ancho desde un slider del GUI
                                   
                                  // raw.setLineW(setLineWidth);   

                        //creator.createRaw(raw);
                         

                        // CREA UN ACTOR B
                        //auto bb =  ActorCreator::create(builder,stateInfo, 10 ,0,"STATIC");
                        //radial_scene->addChild(true,bb->node);

                        registry.radial_scene = radial_scene;       // Registra


                        abstract_nodes->addChild(false, radial_scene);


                        




    /*--------------- radial Depth SCENE -----------------*/

                        auto radiald_scene = vsg::Switch::create();
                        creator.setScene(radiald_scene);
                        creator.createRadialDepth(20, 20); // Crea un radial

                        abstract_nodes->addChild(false, radiald_scene);

    /*--------------- spherical SCENE -----------------*/

                        auto spherical_scene = vsg::Switch::create();
                        creator.setScene(spherical_scene);
                        creator.createSpherical(1, 1); // Crea un Sphere

                        abstract_nodes->addChild(false, spherical_scene);

    /*--------------- cylindrical SCENE -----------------*/

                        auto cylindrical_scene = vsg::Switch::create();
                        creator.setScene(cylindrical_scene);
                        creator.createCylindrical(1, 1); // Crea un Sphere

                        abstract_nodes->addChild(false, cylindrical_scene);

                        /* AGREGA TODOS A ABSTRACT_SCENE */

                        abstract_scene->addChild(true, abstract_nodes);

    /*----------------------------------------------------------------------

            ESCENA, Elementos de interaccion  GUI

    -----------------------------------------------------------------------*/

    // PREPARA LISTENER (Teclado)

    InputKeyboardValues keyb;

    // GUI,  CREA UN NODO ImGui

    auto params = Params::create();
    // auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options));
    auto renderImGui = vsgImGui::RenderImGui::create(window, Main_menu::create(params, options, setLineWidth, &keyb, &scenecontroller, &commandProcessor));

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

    // 25.686613 -100.316116

    auto trackball = vsg::Trackball::create(camera_geo, ellipsoidModel);
    trackball->addKeyViewpoint(vsg::KeySymbol('1'), 25.686613, -100.316116, 2000.0, 2.0);                  // Monterrey
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
    // auto commandGraph = vsg::createCommandGraphForView(window, camera_abstract, abstract_scene);
    // viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

    // add the view handler for interactively changing the views
    // viewer->addEventHandler(ViewHandler::create(renderGraph));

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

    double frameCount = 0.0;
    double radius = 1000000;

    while (viewer->advanceToNextFrame() && (numFrames < 0 || (numFrames--) > 0))
    {
         ++frameCount;

        viewer->handleEvents();

        viewer->update();

        if(a->isDynamic){
            transformEngine.transforma(a, frameCount, radius);
        }



        viewer->recordAndSubmit();

        viewer->present();
    }

    return 0;
}
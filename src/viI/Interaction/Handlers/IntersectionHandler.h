
#include <iostream>
#include <vsg/all.h>



/*----------------------------------------------------


    IntersectionHandler, Manejador de Intersecciones





----------------------------------------------------*/




class IntersectionHandler : public vsg::Inherit<vsg::Visitor, IntersectionHandler>
{
public:
    vsg::GeometryInfo geom;
    vsg::StateInfo state;

    vsg::ref_ptr<vsg::Builder> builder;
    vsg::ref_ptr<vsg::Options> options;
    vsg::ref_ptr<vsg::Camera> camera;
    vsg::ref_ptr<vsg::Switch> scenegraph;
    vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel;
    double scale = 1.0;
    bool verbose = true;


    // Constructor : Recibe un builder para poder crear objetos en demanda.

    IntersectionHandler(vsg::ref_ptr<vsg::Builder> in_builder, vsg::ref_ptr<vsg::Camera> in_camera, vsg::ref_ptr<vsg::Switch> in_scenegraph, vsg::ref_ptr<vsg::EllipsoidModel> in_ellipsoidModel, double in_scale, vsg::ref_ptr<vsg::Options> in_options) :
        builder(in_builder),
        options(in_options),
        camera(in_camera),
        scenegraph(in_scenegraph),
        ellipsoidModel(in_ellipsoidModel),
        scale(in_scale)
    {
        builder->verbose = verbose;
      //  if (scale > 10.0) scale = 10.0;
    }




    // Aplica a eventos de teclado .

    void apply(vsg::KeyPressEvent& keyPress) override
    {
        if (lastPointerEvent)
        {


            // Checamos primero si hay interseccion :

            intersection(*lastPointerEvent);
            if (!lastIntersection) return;

            vsg::info("keyPress.keyModifier = ", keyPress.keyModifier, " keyPress.keyBase = ", keyPress.keyBase);

            geom.dx.set(scale, 0.0f, 0.0f);
            geom.dy.set(0.0f, scale, 0.0f);
            geom.dz.set(0.0f, 0.0f, scale);



            // Calcula psocion para, dependiendo de la tecla, crear un nuevo elemento

            if (keyPress.keyModifier == vsg::MODKEY_Control)
            {
                // when we press the ctrl key we want to enable billboard of the created shapes
                state.billboard = true;

                // when billboard the position is pivot point in local cooridnates
                geom.position.set(0.0f, 0.0f, 0.0f);

                // the position is set by positions data, in this case just one poistion so use a vec4Value, but we can if need use a array of positions
                auto pos = vsg::vec3(lastIntersection->worldIntersection);
                geom.positions = vsg::vec4Value::create(vsg::vec4(pos.x, pos.y, pos.z, scale*5.0)); // x,y,z and scaleDistance
            }
            else
            {
                geom.position = vsg::vec3(lastIntersection->worldIntersection);
            }


            // Dependiendo de la tecla presionada, crea un elemento primitivo usando el builder :
            // el elmento es creado usando la posicion (geom) y el state (solo se usa si es billboard)

            if (keyPress.keyBase == 'b')
            {
                std::cout << "presionaron b" << std::endl;
                scenegraph->addChild(true, builder->createBox(geom, state));
            }
            else if (keyPress.keyBase == 'q')
            {
                scenegraph->addChild(true, builder->createQuad(geom, state));
            }
            else if (keyPress.keyBase == 'c')
            {
                scenegraph->addChild(true, builder->createCylinder(geom, state));
            }
            else if (keyPress.keyBase == 'p')
            {
                scenegraph->addChild(true, builder->createCapsule(geom, state));
            }
            else if (keyPress.keyBase == 'x')
            {
                scenegraph->addChild(true, builder->createSphere(geom, state));
            }
            else if (keyPress.keyBase == 'n')
            {
                scenegraph->addChild(true, builder->createCone(geom, state));
            }
            else if (keyPress.keyBase == 'o')
            {
                vsg::write(scenegraph, "builder.vsgt");
            }
        }

        if (state.billboard)
        {
            // switch off billboarding so other shapes aren't affected.
            state.billboard = false;
            geom.positions = {};
        }
    }




    // Si el boton 1 del mouse es presionado, sensa por intersecciones :

    void apply(vsg::ButtonPressEvent& buttonPressEvent) override
    {
        lastPointerEvent = &buttonPressEvent;

        if (buttonPressEvent.button == 1)
        {
            intersection(buttonPressEvent);
        }
    }


    // Siempre guarda el ultimo evento 

    void apply(vsg::PointerEvent& pointerEvent) override
    {
        lastPointerEvent = &pointerEvent;
    }


    // Sensa por intersecciones :

    void intersection(vsg::PointerEvent& pointerEvent)
    {

        // Crea un LineSegmentIntersector, que basicamente es un rayTracer que va desde la posicion de la camara
        // y se dirige hacia la posicion del mouse transformada a WORLD COORDINATES.

        auto intersector = vsg::LineSegmentIntersector::create(*camera, pointerEvent.x, pointerEvent.y);

        // realiza un traverse de la escena y sus nodos (todos implementan IVisitable)
        scenegraph->accept(*intersector);

        // Haya o no intersecciones, se imprime la posicion actual del mouse
        if (verbose) std::cout << "intersection(" << pointerEvent.x << ", " << pointerEvent.y << ") " << intersector->intersections.size() << ")" << std::endl;


        // Si no hubo intersecciones, termina.
        if (intersector->intersections.empty()) return;

        // Si hubo, ordena las intersecciones para saber quen estuvo primero :
        std::sort(intersector->intersections.begin(), intersector->intersections.end(), [](auto& lhs, auto& rhs) { return lhs->ratio < rhs->ratio; });


        // Recorre las intersecciones ordenadas
        for (auto& intersection : intersector->intersections)
        {
            // imprime los datos de la interseccion en WORLD COORDINATES
            if (verbose) std::cout << "intersection = world(" << intersection->worldIntersection << "), instanceIndex " <<intersection->instanceIndex;

            // Si el modelo es elipsoide, en modo latitud/longitud
            if (ellipsoidModel)
            {
                std::cout.precision(10);
                auto location = ellipsoidModel->convertECEFToLatLongAltitude(intersection->worldIntersection);
                if (verbose) std::cout << " lat = " << location[0] << ", long = " << location[1] << ", height = " << location[2];
            }

            // Si hubo interseccion anterior, calcula la distancia de esa hacia la interseccion actual
            if (lastIntersection)
            {
                if (verbose) std::cout << ", distance from previous intersection = " << vsg::length(intersection->worldIntersection - lastIntersection->worldIntersection);
            }

            if (verbose)
            {
                std::string name;
                for (auto& node : intersection->nodePath)
                {
                    std::cout << ", " << node->className();
                    if (node->getValue("name", name)) std::cout<<":name="<<name;
                }

                std::cout << ", Arrays[ ";
                for (auto& array : intersection->arrays)
                {
                    std::cout << array << " ";
                }
                std::cout << "] [";
                for (auto& ir : intersection->indexRatios)
                {
                    std::cout << "{" << ir.index << ", " << ir.ratio << "} ";
                }
                std::cout << "]";

                std::cout << std::endl;
            }
        }


        // Asume la ultima interseccion como el elemento mas al frente :
        lastIntersection = intersector->intersections.front();
    }

protected:
    vsg::ref_ptr<vsg::PointerEvent> lastPointerEvent;
    vsg::ref_ptr<vsg::LineSegmentIntersector::Intersection> lastIntersection;
};


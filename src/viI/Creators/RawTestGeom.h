
#include "RawGeometry.h"




class RawTestGeom : public RawGeometry {



public:

   RawTestGeom(vsg::ref_ptr<vsg::Options> options){

         // set up pass of material


                        vsg::ref_ptr<vsg::ShaderSet> shaderSet;
                        shaderSet = vsg::createPhongShaderSet(options);

                        // enable wireframe mode to visualize line width
                        auto rasterizationState = vsg::RasterizationState::create();

                        rasterizationState->cullMode = VK_CULL_MODE_NONE;
                        
                        rasterizationState->polygonMode = VK_POLYGON_MODE_LINE;
                        shaderSet->defaultGraphicsPipelineStates.push_back(rasterizationState);

                        if (!shaderSet)
                        {
                            std::cout << "Could not create shaders." << std::endl;
                            //return 1;
                        } else {
                            setShaderSet(shaderSet);
                        }

                                
                              auto vertices = vsg::vec3Array::create(
                                    {{-10000000.5f, 0.0f, 0.0f},
                                    {10000000.5f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f}});


                                setVertex(vertices);

                                auto normals = vsg::vec3Array::create(
                                    {{0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f}});


                                setNormals(normals);

                                auto texcoords = vsg::vec2Array::create(
                                    {{-0.5f, 0.0f},
                                    {0.5f, 0.0f},
                                    {0.0f, 0.0f}});


                                setTexCoords(texcoords);


                                auto colors = vsg::vec4Value::create(vsg::vec4{1.0f, 0.0f, 0.0f, 1.0f});


                                setColors(colors);


                                auto indices = vsg::ushortArray::create(
                                    {0, 1, 2,
                                    0});


                                setIndices(indices);
       

   }






};

/*
 // set up vertex and index arrays
                                auto vertices = vsg::vec3Array::create(
                                    {{-0.5f, -0.5f, 0.0f},
                                    {0.5f, -0.5f, 0.0f},
                                    {0.5f, 0.5f, 0.0f},
                                    {-0.5f, 0.5f, 0.0f},
                                    {-0.5f, -0.5f, -0.5f},
                                    {0.5f, -0.5f, -0.5f},
                                    {0.5f, 0.5f, -0.5f},
                                    {-0.5f, 0.5f, -0.5f}});


                                setVertex(vertices);

                                auto normals = vsg::vec3Array::create(
                                    {{0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f}});


                                setNormals(normals);

                                auto texcoords = vsg::vec2Array::create(
                                    {{0.0f, 0.0f},
                                    {1.0f, 0.0f},
                                    {1.0f, 1.0f},
                                    {0.0f, 1.0f},
                                    {0.0f, 0.0f},
                                    {1.0f, 0.0f},
                                    {1.0f, 1.0f},
                                    {0.0f, 1.0f}});


                                setTexCoords(texcoords);


                                auto colors = vsg::vec4Value::create(vsg::vec4{1.0f, 1.0f, 1.0f, 1.0f});


                                setColors(colors);


                                auto indices = vsg::ushortArray::create(
                                    {0, 1, 2,
                                    2, 3, 0,
                                    4, 5, 6,
                                    6, 7, 4});


                                setIndices(indices);




                                auto vertices = vsg::vec3Array::create(
                                    {{-0.5f, 0.0f, 0.0f},
                                    {0.5f, 0.0f, 0.0f},
                                    {0.0f, 0.0f, 0.0f}});


                                setVertex(vertices);

                                auto normals = vsg::vec3Array::create(
                                    {{0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f},
                                    {0.0f, 0.0f, 1.0f}});


                                setNormals(normals);

                                auto texcoords = vsg::vec2Array::create(
                                    {{-0.5f, 0.0f},
                                    {0.5f, 0.0f},
                                    {0.0f, 0.0f}});


                                setTexCoords(texcoords);


                                auto colors = vsg::vec4Value::create(vsg::vec4{1.0f, 0.0f, 0.0f, 1.0f});


                                setColors(colors);


                                auto indices = vsg::ushortArray::create(
                                    {0, 1, 2,
                                    0});


                                setIndices(indices);
       





*/
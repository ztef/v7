#include "RawGeometry.h"




class Line : public RawGeometry {



public:

   Line(float x0, float y0, float z0, float x1, float y1, float z1, vsg::ref_ptr<vsg::Options> options){

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
                                    {{x0, y0, z0},
                                    {x1, y1, z1},
                                    {x0, y0, z0}});


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

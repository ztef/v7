#include <vsg/all.h>
#include "RawGeometry.h"





class PipeLineProcessor {

public:

    void transform(RawGeometry &r){

        r.graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(r.shaderSet);

        // instantiate dynamicstate and add the state
        
        r.graphicsPipelineConfig->dynamicState = vsg::DynamicState::create();
        r.graphicsPipelineConfig->dynamicState->dynamicStates.emplace_back(VK_DYNAMIC_STATE_LINE_WIDTH);


        r.graphicsPipelineConfig->assignUniform(r.descriptors, "material", r.material);


        r.graphicsPipelineConfig->assignArray(r.vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, r.vertex);
        r.graphicsPipelineConfig->assignArray(r.vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, r.normals);
        r.graphicsPipelineConfig->assignArray(r.vertexArrays, "vsg_TexCoord0", VK_VERTEX_INPUT_RATE_VERTEX, r.texcoords);
        r.graphicsPipelineConfig->assignArray(r.vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, r.colors);


        r.drawCommands = vsg::Commands::create();
        r.drawCommands->addChild(vsg::BindVertexBuffers::create(r.graphicsPipelineConfig->baseAttributeBinding, r.vertexArrays));
        r.drawCommands->addChild(vsg::BindIndexBuffer::create(r.indices));
        r.drawCommands->addChild(vsg::DrawIndexed::create(12, 1, 0, 0, 0));

        if(r.setLineWidth){
        r.drawCommands->addChild(r.setLineWidth);
        }


        // register the ViewDescriptorSetLayout.
                    
                    
        r.vdsl = vsg::ViewDescriptorSetLayout::create();
        r.graphicsPipelineConfig->additionalDescriptorSetLayout = r.vdsl;


        // share the pipeline config and initilaize if it's unique
                    
            r.graphicsPipelineConfig->init();

           r.descriptorSet = vsg::DescriptorSet::create(r.graphicsPipelineConfig->descriptorSetLayout, r.descriptors);
                    
            r.bindDescriptorSet = vsg::BindDescriptorSet::create(VK_PIPELINE_BIND_POINT_GRAPHICS, r.graphicsPipelineConfig->layout, 0, r.descriptorSet);
                    
            r.bindViewDescriptorSets = vsg::BindViewDescriptorSets::create(VK_PIPELINE_BIND_POINT_GRAPHICS, r.graphicsPipelineConfig->layout, 1);
                    

                 
        // create StateGroup as the root of the scene/command graph to hold the GraphicsProgram, and binding of Descriptors to decorate the whole graph
            r.stateGroup = vsg::StateGroup::create();
            r.stateGroup->add(r.graphicsPipelineConfig->bindGraphicsPipeline);
            r.stateGroup->add(r.bindDescriptorSet);
            r.stateGroup->add(r.bindViewDescriptorSets);

        // set up model transformation node

            r.transform = vsg::MatrixTransform::create();
            r.transform->subgraphRequiresLocalFrustum = false;

            r.stateGroup->addChild(r.drawCommands);
            r.transform->addChild(r.stateGroup);                    //Producto final del PipeLine.





    };


};

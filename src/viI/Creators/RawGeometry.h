#pragma once
#include <vsg/all.h>


class RawGeometry {


    public :

    RawGeometry();

    vsg::ref_ptr<vsg::GraphicsPipelineConfigurator> graphicsPipelineConfig;
    vsg::Descriptors descriptors;
     vsg::DataList vertexArrays;


    vsg::ref_ptr<vsg::ShaderSet> shaderSet;

    vsg::ref_ptr<vsg::PhongMaterialValue> material;
    vsg::ref_ptr<vsg::vec3Array> vertex;
    vsg::ref_ptr<vsg::vec3Array> normals;
    vsg::ref_ptr<vsg::vec2Array> texcoords;
    vsg::ref_ptr<vsg::ushortArray> indices;
    vsg::ref_ptr<vsg::vec4Value> colors;

    vsg::ref_ptr<vsg::SetLineWidth> setLineWidth;

    vsg::ref_ptr<vsg::Commands> drawCommands;
    vsg::ref_ptr<vsg::ViewDescriptorSetLayout> vdsl;
    vsg::ref_ptr<vsg::DescriptorSet> descriptorSet;
    vsg::ref_ptr<vsg::BindDescriptorSet> bindDescriptorSet;
    vsg::ref_ptr<vsg::BindViewDescriptorSets> bindViewDescriptorSets;
    vsg::ref_ptr<vsg::StateGroup> stateGroup;

    vsg::ref_ptr<vsg::MatrixTransform> transform;


   

    void setShaderSet(vsg::ref_ptr<vsg::ShaderSet> shaderSet); 
    void setMaterial(vsg::ref_ptr<vsg::PhongMaterialValue> mat);
    void setVertex(vsg::ref_ptr<vsg::vec3Array> v);
    void setNormals(vsg::ref_ptr<vsg::vec3Array> normals);
    void setTexCoords(vsg::ref_ptr<vsg::vec2Array> texcoords);
    void setIndices(vsg::ref_ptr<vsg::ushortArray> indices);
    void setColors(vsg::ref_ptr<vsg::vec4Value> colors);
    void setLineW(vsg::ref_ptr<vsg::SetLineWidth> setLineWidth);

};
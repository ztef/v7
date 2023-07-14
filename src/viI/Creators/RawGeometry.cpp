#include "RawGeometry.h"



RawGeometry::RawGeometry(){


}



 void RawGeometry::setLineW(vsg::ref_ptr<vsg::SetLineWidth> lw){
    setLineWidth = lw;
 };


 void RawGeometry::setShaderSet(vsg::ref_ptr<vsg::ShaderSet> sh){
    shaderSet = sh;
 }; 

void RawGeometry::setMaterial(vsg::ref_ptr<vsg::PhongMaterialValue> m){
   material = m;
};

void RawGeometry::setVertex(vsg::ref_ptr<vsg::vec3Array> v){
    vertex = v;
};

void RawGeometry::setNormals(vsg::ref_ptr<vsg::vec3Array> n){
    normals = n;
};

void RawGeometry::setTexCoords(vsg::ref_ptr<vsg::vec2Array> t){
    texcoords = t;

};
void RawGeometry::setIndices(vsg::ref_ptr<vsg::ushortArray> i){
    indices = i;

};
void RawGeometry::setColors(vsg::ref_ptr<vsg::vec4Value> c){
    colors = c;
};


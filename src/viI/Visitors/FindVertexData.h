#pragma once
#include <vsg/all.h>




class FindVertexData : public vsg::Visitor
{
public:

    void apply(vsg::Object& object)
    {
        object.traverse(*this);
    }

    void apply(vsg::Geometry& geometry)
    {
        if (geometry.arrays.empty()) return;
        geometry.arrays[0]->data->accept(*this);
    }

    void apply(vsg::VertexIndexDraw& vid)
    {
        if (vid.arrays.empty()) return;
        vid.arrays[0]->data->accept(*this);
    }

    void apply(vsg::BindVertexBuffers& bvd)
    {
        if (bvd.arrays.empty()) return;
        bvd.arrays[0]->data->accept(*this);
    }

    void apply(vsg::vec3Array& vertices)
    {
        if (verticesSet.count(&vertices) == 0)
        {
            verticesSet.insert(&vertices);
        }
    }


    std::vector<vsg::ref_ptr<vsg::vec3Array>> getVerticesList()
    {
        std::vector<vsg::ref_ptr<vsg::vec3Array>> verticesList(verticesSet.size());
        auto vertices_itr = verticesList.begin();
        for(auto& vertices : verticesSet)
        {
            (*vertices_itr++) = const_cast<vsg::vec3Array*>(vertices);
        }

        return verticesList;
    }

    std::set<vsg::vec3Array*> verticesSet;
};



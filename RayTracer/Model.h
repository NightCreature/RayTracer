#pragma once


#include "MeshGroup.h"
#include <vector>


class Model
{
public:

    Model() {}
    ~Model() 
    {
    }

    void addMeshGroup(const MeshGroup& meshGroup) { m_modelData.push_back(meshGroup); }


    std::vector<MeshGroup> m_modelData;
};

struct CreatedModel
{
    Model* model;
};

struct CreatedMeshGroup
{
    MeshGroup* meshGroup;
};
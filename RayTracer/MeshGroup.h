#pragma once

#include "Material.h"
#include "Math/matrix44.h"

#include <vector>

struct Face
{
    size_t m_index1 = 0;
    size_t m_index2 = 1;
    size_t m_index3 = 2;
};

//Wrapper to hold more then one Geometry instance and material together
class MeshGroup
{
public:
    MeshGroup()
    {
    }
    MeshGroup(const MeshGroup& source);
    ~MeshGroup();
   
    std::vector<Vector3> m_vertices;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_uvs;
    std::vector<Face> m_faces;

    Material m_material;
};


#include "MeshGroupCreator.h"

#include "assert.h"

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedMeshGroup MeshGroupCreator::CreateMeshGroup(const CreationParams& params)
{
    CreatedMeshGroup meshGroup;

    unsigned int m_nummultitexcoords = 1; //HACK FIX THIS
    meshGroup.meshGroup = new MeshGroup();
    meshGroup.meshGroup->m_vertices.resize(params.m_vertices.size());
    meshGroup.meshGroup->m_vertices.assign(params.m_vertices.begin(), params.m_vertices.end());
    meshGroup.meshGroup->m_normals.resize(params.m_normals.size());
    meshGroup.meshGroup->m_normals.assign(params.m_normals.begin(), params.m_normals.end());
    //meshGroup.meshGroup->m_uvs.resize(m_nummultitexcoords * params.m_texcoords.size());

    //Bounding box setup might be handy later
    //meshGroup.boundingBox.enclose(params.m_vertices[counter]);

    return meshGroup;
}

void MeshGroupCreator::normalizeNormals(std::vector<Vector3>& normals)
{
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i].normalize();
    }
}
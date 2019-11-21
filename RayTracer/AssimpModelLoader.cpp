#include "AssimpModelLoader.h"
#include "mesh.h"
#include "MeshGroupCreator.h"

#include <D:\SDK\Demo\SpaceSim\SDK\Assimp\include\assimp/Importer.hpp>      // C++ importer interface
#include <D:\SDK\Demo\SpaceSim\SDK\Assimp\include\assimp/scene.h>           // Output data structure
#include <D:\SDK\Demo\SpaceSim\SDK\Assimp\include\assimp/postprocess.h>     // Post processing flags

namespace AssimpModelLoader
{

///-----------------------------------------------------------------------------
///! @brief   
///! @remark  This needs re writing so we can apply uv transforms to correct streams and embedded correct uv streams in to the vbs.
///-----------------------------------------------------------------------------
CreatedModel LoadModel(const Material& material, const std::string& fileName)
{
    if (fileName.empty())
    {
        return CreatedModel();
    }

    //Load the model here
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing  
    // Usually - if speed is not the most important aspect for you - you'll   
    // propably to request more postprocessing than we do in this example.  aiProcessPreset_TargetRealtime_Quality
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipWindingOrder | aiProcess_FlipUVs); //Optimize the mesh and scenegraph to reduce drawcalls
    if (!scene)
    {
        return CreatedModel();
    }  // Now we can access the file's contents.

    MeshGroupCreator::CreationParams params;
    //Grab the verts here
    unsigned int highestIndex = 0;
    //Extract vertices from the mesh here and store in our own vertex buffer
    for (size_t meshCounter = 0; meshCounter < scene->mNumMeshes; ++meshCounter)
    {
        MeshGroupCreator::CreationParams meshGroupParams;
        aiMesh* subMesh = scene->mMeshes[meshCounter];
        for (size_t vertCounter = 0; vertCounter < subMesh->mNumVertices; ++vertCounter)
        {
            meshGroupParams.m_vertices.push_back(Vector3(subMesh->mVertices[vertCounter].x, subMesh->mVertices[vertCounter].y, subMesh->mVertices[vertCounter].z));

            if (subMesh->HasNormals())
            {
                meshGroupParams.m_normals.push_back(Vector3(-subMesh->mNormals[vertCounter].x, -subMesh->mNormals[vertCounter].y, -subMesh->mNormals[vertCounter].z));
                //MSG_TRACE_CHANNEL("NORMAL DEBUG", "Normal: %f, %f, %f", subMesh->mNormals[vertCounter].x, subMesh->mNormals[vertCounter].y, subMesh->mNormals[vertCounter].z);
            }
            if (subMesh->mTangents != nullptr)
            {
                meshGroupParams.m_tangents.push_back(Vector3(-subMesh->mTangents[vertCounter].x, -subMesh->mTangents[vertCounter].y, -subMesh->mTangents[vertCounter].z));
            }
            if (subMesh->mBitangents != nullptr)
            {
                meshGroupParams.m_tangents.push_back(Vector3(-subMesh->mBitangents[vertCounter].x, -subMesh->mBitangents[vertCounter].y, -subMesh->mBitangents[vertCounter].z));
            }

            for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
            {
                if (subMesh->HasTextureCoords(uvChannel))
                {
                    aiVector3D* texCoordChannel = subMesh->mTextureCoords[uvChannel];
                    while (meshGroupParams.m_texcoords.size() <= uvChannel)
                    {
                        meshGroupParams.m_texcoords.push_back(std::vector<Vector3>());
                    }
                    switch (subMesh->mNumUVComponents[uvChannel])
                    {
                    case 1:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, 0.0f, 0.0f);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 2:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, 0.0f);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 3:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, texCoordChannel[vertCounter].z);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];

                        smit.push_back(vec);

                        //    uvs.push_back(texCoordChannel[vertCounter].x);
                        //    uvs.push_back(texCoordChannel[vertCounter].y);
                        //    uvs.push_back(texCoordChannel[vertCounter].z);
                    }
                    break;
                    default:
                        break;
                    }
                }
            }
        }

        //Need to keep track of highest index and add it to the next batch and so one sadly
        unsigned int baseIndexOffset = highestIndex;
        for (size_t indexCounter = 0; indexCounter < subMesh->mNumFaces; ++indexCounter)
        {
            for (size_t counterIndex = 0; counterIndex < subMesh->mFaces[indexCounter].mNumIndices; ++counterIndex)
            {
                meshGroupParams.m_indices.push_back(subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset);
                if (subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset > highestIndex)
                {
                    highestIndex = subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset;
                }
            }
        }

        aiMaterial* aimaterial = scene->mMaterials[subMesh->mMaterialIndex];
        Material shaderMaterial;// = meshGroupParams.m_shaderInstance.getMaterial();
        aiColor4D color;
        aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        shaderMaterial.SetDiffuseColor( Vector4(color.r, color.g, color.b, color.a) );
        
        float refractionIndex = 0.0f;
        aimaterial->Get(AI_MATKEY_REFRACTI, refractionIndex);
        shaderMaterial.SetRefractionIndex( refractionIndex);
        shaderMaterial.SetType( refractionIndex > 0 ? MaterialType::refractive : MaterialType::diffuse);
        
        

        
        //Add mesh group to the mesh creator params
        //params.m_meshGroups.push_back(MeshGroupCreator::CreateMeshGroup(meshGroupParams));
    }



    return CreatedModel(); //Mesh::CreateMesh(params);
}

}
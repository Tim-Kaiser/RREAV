#include "../include/ObjectLoader.h"

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

bool loadObject(const std::string& path, Object& obj)
{
    std::vector<glm::vec3>& pos;
    std::vector<glm::vec3>& normal;
    std::vector<glm::vec2>& uv;


    Assimp::Importer importer;

    const aiScene* scene = importer.readFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if(!scene)
    {
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    const aiVector3D nullVec(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        const aiVector3D* vertPos = &(aiMesh->mVertices[i]);
        const aiVector3D* vertNormal = &(aiMesh->mNormals[i]) : &nullVec;
        const aiVector3D* vertUV = aiMesh->hasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &nullVec;

        pos.push_back(glm::vec3(vertPos->x, vertPos->y, vertPos->z));
        normal.push_back(glm::vec3(vertNormal->x, vertNormal->y, vertNormal->z));
        uv.push_back(glm::vec3(vertUV->x, vertUV->y));
    }

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace* face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
        {
            
        }
    }
    
    return true;
}

void indexing(std::vector<size_t>& vertexIndices, std::vector<size_t>& uvIndices, std::vector<size_t>& normalIndices, std::vector<glm::vec3>& temp_v, std::vector<glm::vec2>& temp_uv, std::vector<glm::vec3>& temp_n, Object& obj)
{
    obj.vertices.reserve(vertexIndices.size());
    obj.normals.reserve(normalIndices.size());
    obj.uvs.reserve(uvIndices.size());

    for (int i = 0; i < vertexIndices.size(); i++)
    {
        size_t vi = vertexIndices[i];
        glm::vec3 vertex = temp_v[vi - 1];
        obj.vertices.emplace_back((GLfloat)vertex.x);
        obj.vertices.emplace_back((GLfloat)vertex.y);
        obj.vertices.emplace_back((GLfloat)vertex.z);
    }
    for (int i = 0; i < uvIndices.size(); i++)
    {
        size_t uvi = uvIndices[i];
        glm::vec2 uv = temp_uv[uvi - 1];
        obj.uvs.emplace_back((GLfloat)uv.x);
        obj.uvs.emplace_back((GLfloat)uv.y);
    }
    for (int i = 0; i < normalIndices.size(); i++)
    {
        size_t ni = normalIndices[i];
        glm::vec3 normal = temp_n[ni - 1];
        obj.normals.emplace_back(normal.x);
        obj.normals.emplace_back(normal.y);
        obj.normals.emplace_back(normal.z);
    }
}


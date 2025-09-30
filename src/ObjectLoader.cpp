#include "../include/rreav/ObjectLoader.h"

Mesh loadObject(const std::string &path) {

  std::vector<GLuint> indices;
  std::vector<Vertex> vertices;

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  const aiMesh *mesh = scene->mMeshes[0];

  const aiVector3D nullVec(0.0f, 0.0f, 0.0f);

  for (int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;

    const aiVector3D *vertPos = &(mesh->mVertices[i]);
    const aiVector3D *vertNormal =
        &(mesh->mNormals[i]) ? &(mesh->mNormals[i]) : &nullVec;
    const aiVector3D *vertUV =
        mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &nullVec;

    pos.x = vertPos->x;
    pos.y = vertPos->y;
    pos.z = vertPos->z;
    vertex.position = pos;

    normal.x = vertNormal->x;
    normal.y = vertNormal->y;
    normal.z = vertNormal->z;
    vertex.normal = normal;

    uv.x = vertUV->x;
    uv.y = vertUV->y;
    vertex.uv = uv;

    vertices.push_back(vertex);
  }

  for (int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace *face = &(mesh->mFaces[i]);
    for (int j = 0; j < face->mNumIndices; j++) {
      indices.push_back(face->mIndices[j]);
    }
  }

  return Mesh(vertices, indices);
}

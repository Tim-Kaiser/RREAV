#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glad/gl.h"

struct Object {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;

	Object() = default;
	Object& operator=(const Object& rhs)
	{
		if (this != &rhs)
		{
			vertices = rhs.vertices;
			uvs = rhs.uvs;
			normals = rhs.normals;
		}
		return *this;
	}
};

bool loadObject(const std::string& path, Object& obj);
void indexing(std::vector< size_t >& vertexIndices, std::vector< size_t >& uvIndices, std::vector< size_t >& normalIndices, std::vector< glm::vec3 >& temp_v, std::vector< glm::vec2 >& temp_uv, std::vector< glm::vec3 >& temp_n, Object& obj );
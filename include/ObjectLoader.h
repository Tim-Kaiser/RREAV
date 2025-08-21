#pragma once
#include "Mesh.h"
#include "glad/gl.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

Mesh loadObject(const std::string &path);

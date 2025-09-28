#pragma once

#include "OpenGL/opengl.h"
#include <glm/glm.hpp>
#include <vector>

typedef struct {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
} Vertex;

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
  void render();

private:
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;

  bool init();
};
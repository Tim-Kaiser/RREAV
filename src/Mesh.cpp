#include "../include/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
    : m_vertices(vertices), m_indices(indices) {

  init();
}

bool Mesh::init() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
               m_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, uv));
  glBindVertexArray(0);

  // TODO: Error checking
  return true;
}

void Mesh::render() {
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
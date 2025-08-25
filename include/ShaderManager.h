#pragma once

#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

typedef struct {
  GLuint m_shaderProgramID;
  GLuint m_vertexShaderID;
  GLuint m_fragmentShaderID;

} Shader;

typedef struct {
  GLuint m_shaderProgramID;
  GLuint m_computeShaderID;
} ComputeShader;

class ShaderManager {

public:
  // === SHADER ===
  ~ShaderManager();
  std::unique_ptr<Shader>
  CreateShaders(const std::string &vertexShaderFilename,
                const std::string &fragmentShaderFilename);

  void DetachShaders(Shader &shader);
  void DestroyShaders(Shader &shader);
  void DestroyProgram(GLuint &shaderProgramID);

  // === COMPUTE SHADER ===

  std::unique_ptr<ComputeShader>
  CreateComputeShader(const std::string &computeShaderFilename);

  void DetachShaders(ComputeShader &shader);
  void DestroyShaders(ComputeShader &shader);

  bool SendUniformData(const std::string &uniformName, GLint &data);
  bool SendUniformData(const std::string &uniformName, GLfloat &data);
  bool SendUniformData(const std::string &uniformName, GLuint &data);

  bool SendUniformData(const std::string &uniformName, GLfloat &x, GLfloat &y);
  bool SendUniformData(const std::string &uniformName, GLfloat &x, GLfloat &y,
                       GLfloat &z);
  bool SendUniformData(const std::string &uniformName, GLfloat &x, GLfloat &y,
                       GLfloat &z, GLfloat &w);

  bool SendUniformData(const std::string &uniformName, const glm::mat4 &data);
  bool SendUniformData(const std::string &uniformName, const glm::vec3 &data);

private:
  bool CompileShaders(const std::string &filename, GLuint &shaderID);
  void AttachShaders(Shader &shader);
  bool LinkProgram(GLuint &shaderProgramID);

  void AttachShaders(ComputeShader &shader);

  GLint GetUniformID(const std::string &uniformName);

  std::vector<Shader> m_createdShaders;
  std::vector<ComputeShader> m_createdComputeShaders;
};
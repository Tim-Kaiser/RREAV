#include <fstream>
#include <iostream>
#include "../include/ShaderManager.h"


ShaderManager::~ShaderManager()
{
    for (Shader s : m_createdShaders)
    {
        DetachShaders(s);
        DestroyShaders(s);
        DestroyProgram(s.m_shaderProgramID);
        std::cout << "Successfully destroyed shader" << std::endl;
    }

    for (ComputeShader s : m_createdComputeShaders)
    {
        DetachShaders(s);
        DestroyShaders(s);
        DestroyProgram(s.m_shaderProgramID);
        std::cout << "Successfully destroyed compute shader" << std::endl;

    }
}

std::unique_ptr<Shader> ShaderManager::CreateShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShaderID == 0) {
        std::cout << "Error creating vertex shader object" << std::endl;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (vertexShaderID == 0) {
        std::cout << "Error creating fragment shader object" << std::endl;
    }
    std::unique_ptr<Shader> shader(new Shader);
    shader->m_shaderProgramID = glCreateProgram();
    shader->m_fragmentShaderID = fragmentShaderID;
    shader->m_vertexShaderID = vertexShaderID;


    CompileShaders(vertexShaderFilename, shader->m_vertexShaderID);
    CompileShaders(fragmentShaderFilename, shader->m_fragmentShaderID);
    AttachShaders(*shader);
    LinkProgram(shader->m_shaderProgramID);

    m_createdShaders.push_back(*shader);

    return shader;
}

bool ShaderManager::CompileShaders(const std::string& filename, GLuint& shaderID)
{
    std::fstream file;
    std::string shaderCode;
    std::string line;

    file.open(filename);

    if (!file) {
        std::cout << "Error reading shader file: " << filename << std::endl;
        return false;
    }

    while (!file.eof())
    {
        std::getline(file, line);
        shaderCode += line + "\n";
    }
    file.close();

    const GLchar* src = reinterpret_cast<const GLchar*>(shaderCode.c_str());

    // pass in the shader ID (either vertex or fragment), the amount of shaders, the pointer to the source code, and NULL(only relevant for multiple shaders at the same time)
    glShaderSource(shaderID, 1, &src, nullptr);

    glCompileShader(shaderID);

    GLint errorCode;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &errorCode);

    if (errorCode == GL_TRUE) {
        std::cout << "Shader compiled successfully" << std::endl;
    }
    else {
        GLsizei bufferSize = 1000;
        GLchar errorMessage[1000];

        glGetShaderInfoLog(shaderID, bufferSize, &bufferSize, errorMessage);
        std::cout << errorMessage << std::endl;
        return false;
    }
    return true;
}

void ShaderManager::AttachShaders(Shader& shader)
{
    glAttachShader(shader.m_shaderProgramID, shader.m_fragmentShaderID);
    glAttachShader(shader.m_shaderProgramID, shader.m_vertexShaderID);
}

bool ShaderManager::LinkProgram(GLuint& shaderProgramID)
{
    glLinkProgram(shaderProgramID);
    glUseProgram(shaderProgramID);

    GLint errorCode;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &errorCode);

    if (errorCode == GL_TRUE) {
        std::cout << "Program linked successfully" << std::endl;
    }
    else {
        GLsizei bufferSize = 1000;
        GLchar errorMessage[1000];

        glGetProgramInfoLog(shaderProgramID, bufferSize, &bufferSize, errorMessage);
        std::cout << errorMessage << std::endl;
        return false;
    }

    return true;
}


void ShaderManager::DetachShaders(Shader& shader)
{
    glDetachShader(shader.m_shaderProgramID, shader.m_vertexShaderID);
    glDetachShader(shader.m_shaderProgramID, shader.m_fragmentShaderID);
}

void ShaderManager::DestroyShaders(Shader& shader)
{
    glDeleteShader(shader.m_vertexShaderID);
    glDeleteShader(shader.m_fragmentShaderID);
}

void ShaderManager::DestroyProgram(GLuint& shaderProgramID)
{
    glDeleteProgram(shaderProgramID);
}

std::unique_ptr<ComputeShader> ShaderManager::CreateComputeShader(const std::string& computeShaderFilename)
{
    std::unique_ptr<ComputeShader> computeShader(new ComputeShader);

    computeShader->m_shaderProgramID = glCreateProgram();
    computeShader->m_computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    CompileShaders(computeShaderFilename, computeShader->m_computeShaderID);
    AttachShaders(*computeShader);
    LinkProgram(computeShader->m_shaderProgramID);

    m_createdComputeShaders.push_back(*computeShader);

    return computeShader;
}

void ShaderManager::AttachShaders(ComputeShader& shader)
{
    glAttachShader(shader.m_shaderProgramID, shader.m_computeShaderID);
}

void ShaderManager::DetachShaders(ComputeShader& shader)
{
    glDetachShader(shader.m_shaderProgramID, shader.m_computeShaderID);
}

void ShaderManager::DestroyShaders(ComputeShader& shader)
{
    glDeleteShader(shader.m_computeShaderID);
}

GLint ShaderManager::GetUniformID(const std::string& uniformName) {

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    GLint id = glGetUniformLocation(currentProgram, uniformName.c_str());
    if (id == -1) {
        return -1;
    }
    return id;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLint& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform1i(id, data);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLfloat& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform1f(id, data);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLuint& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform1ui(id, data);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform2f(id, x, y);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform3f(id, x, y, z);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, GLfloat& x, GLfloat& y, GLfloat& z, GLfloat& w)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform4f(id, x, y, z, w);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, const glm::mat4& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
    return true;
}

bool ShaderManager::SendUniformData(const std::string& uniformName, const glm::vec3& data)
{
    GLint id = GetUniformID(uniformName);

    if (id == -1) {
        return false;
    }

    glUniform3fv(id, 1, glm::value_ptr(data));
    return true;
}




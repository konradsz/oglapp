#pragma once

#include <string>

#include <GL/glew.h>

class ShaderLoader
{
public:
    ShaderLoader() = default;
    ~ShaderLoader() = default;

    GLuint createProgram(const std::string& vertexShaderPath,
                         const std::string& fragmentShaderPath);

    GLuint id();

private:
    std::string readFromFile(const std::string& fileName);
    GLuint compile(GLenum type, const std::string& source);

private:
    GLuint m_programId;
};

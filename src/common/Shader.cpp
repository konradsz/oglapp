#define GLEW_STATIC
#include <GL/glew.h>

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using dummy::common::Shader;

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    auto vertexShaderCode = readFromFile(vertexShaderPath);
    auto fragmentShaderCode = readFromFile(fragmentShaderPath);

    auto vertexShader = compile(GL_VERTEX_SHADER, vertexShaderCode);
    auto fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderCode);

    m_id = link(vertexShader, fragmentShader);
}

void Shader::use() const
{
    glUseProgram(m_id);
}

GLuint Shader::id() const
{
    return m_id;
}

std::string Shader::readFromFile(const std::string& filePath) const
{
    std::string shaderCode;
    std::ifstream file{filePath, std::ifstream::in};

    if (!file.good())
    {
        std::cerr << "Cannot read shader from file: " << filePath << std::endl;
        std::terminate();
    }

    file.seekg(0, std::ios::end);
    shaderCode.resize(static_cast<unsigned int>(file.tellg()));
    file.seekg(0, std::ios::beg);

    shaderCode.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();

    return shaderCode;
}

GLuint Shader::compile(GLenum type, const std::string& source) const
{
    GLint compileResult = 0;

    GLuint shader = glCreateShader(type);
    const char* sourceCstr = source.c_str();
    const int sourceSize = source.size();

    glShaderSource(shader, 1, &sourceCstr, &sourceSize);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, nullptr, &errorLog[0]);
        std::cerr << "Compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader failed!"
                  << std::endl << &errorLog[0] << std::endl;

        glDeleteShader(shader);

        return -1;
    }

    return shader;
}

GLuint Shader::link(GLuint vertexShader, GLuint fragmentShader) const
{
    GLint linkResult = 0;

    GLuint id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &linkResult);

    if (linkResult == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(id, maxLength, nullptr, &errorLog[0]);
        std::cerr << "Linking shader program failed!" << std::endl << &errorLog[0] << std::endl;

        glDeleteProgram(id);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    glDetachShader(id, vertexShader);
    glDetachShader(id, fragmentShader);

    return id;
}

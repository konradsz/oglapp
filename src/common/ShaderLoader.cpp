#include "ShaderLoader.h"

#include <iostream>
#include <fstream>
#include <vector>

GLuint ShaderLoader::createProgram(const std::string& vertexShaderPath,
                                   const std::string& fragmentShaderPath)
{
    std::string vertexShaderCode = readFromFile(vertexShaderPath);
    std::string fragmentShaderCode = readFromFile(fragmentShaderPath);

    GLuint vertexShader = compile(GL_VERTEX_SHADER, vertexShaderCode);
    GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderCode);

    GLint linkResult = 0;

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertexShader);
    glAttachShader(m_programId, fragmentShader);

    glLinkProgram(m_programId);
    glGetProgramiv(m_programId, GL_LINK_STATUS, &linkResult);

    if (linkResult == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, nullptr, &errorLog[0]);
        std::cerr << "Linking shader program failed!" << std::endl << &errorLog[0] << std::endl;

        glDeleteProgram(m_programId);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return -1;
    }

    glDetachShader(m_programId, vertexShader);
    glDetachShader(m_programId, fragmentShader);

    return m_programId;
}

GLuint ShaderLoader::id()
{
    return m_programId;
}

std::string ShaderLoader::readFromFile(const std::string& filePath)
{
    std::string shaderCode;
    std::ifstream file(filePath, std::ifstream::in);

    if (!file.good())
    {
        std::cerr << "Cannot read shader from file: " << filePath << std::endl;
        std::terminate();
    }

    file.seekg(0, std::ios::end);
    shaderCode.resize(file.tellg());
    file.seekg(0, std::ios::beg);

    shaderCode.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();

    return shaderCode;
}

GLuint ShaderLoader::compile(GLenum type, const std::string& source)
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

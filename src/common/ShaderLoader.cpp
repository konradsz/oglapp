#include "ShaderLoader.h"

#include <iostream>
#include <fstream>
#include <vector>

GLuint ShaderLoader::createProgram(const std::string& vertexShaderPath,
                                   const std::string& fragmentShaderPath)
{
    std::string vertexShaderCode = readFromFile(vertexShaderPath);
    std::string fragmentShaderCode = readFromFile(fragmentShaderPath);

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderCode);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    int linkResult = 0;

    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);

    if (linkResult == GL_FALSE)
    {
        int logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> programLog(logLength);
        glGetProgramInfoLog(programId, logLength, nullptr, &programLog[0]);
        std::cout << "[ShaderLoader] LINK ERROR" << std::endl << &programLog[0] << std::endl;
        return 0;
    }
    return programId;
}

GLuint ShaderLoader::id()
{
    return programId;
}

std::string ShaderLoader::readFromFile(const std::string& filePath)
{
    std::string shaderCode;
    std::ifstream file(filePath, std::ios::in);

    if (!file.good())
    {
        std::cout << "[ShaderLoader] Cannot read file: " << filePath << std::endl;
        std::terminate();
    }

    file.seekg(0, std::ios::end);
    shaderCode.resize((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], shaderCode.size());
    file.close();
    return shaderCode;
}

GLuint ShaderLoader::createShader(GLenum shaderType, const std::string& source)
{
    int compileResult = 0;

    GLuint shader = glCreateShader(shaderType);
    const char* sourceCstr = source.c_str();
    const int sourceSize = source.size();

    glShaderSource(shader, 1, &sourceCstr, &sourceSize);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == GL_FALSE)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> shader_log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, &shader_log[0]);
        std::cout << "[ShaderLoader] ERROR compiling shader! (" << shaderType << ")" 
                  << std::endl << &shader_log[0] << std::endl;
        return 0;
    }
    return shader;
}

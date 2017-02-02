#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using common::ShaderManager;
using common::Shader;

void ShaderManager::createShader(const std::string& shaderName,
                                 const std::string& vertexShaderPath,
                                 const std::string& fragmentShaderPath)
{
    m_shaders[shaderName] = Shader{vertexShaderPath, fragmentShaderPath};
}

Shader ShaderManager::getShader(const std::string& shaderName) const
{
    auto shader = m_shaders.find(shaderName);
    if (shader != std::end(m_shaders))
    {
        return shader->second;
    }
    else
    {
        std::cerr << "Requested shader \"" << shaderName << "\" does not exist!" << std::endl;
        return {};
    }
}

ShaderManager::~ShaderManager()
{
    std::for_each(std::begin(m_shaders), std::end(m_shaders),
        [](const auto& shader) { glDeleteProgram(shader.second.id()); });
    m_shaders.clear();
}

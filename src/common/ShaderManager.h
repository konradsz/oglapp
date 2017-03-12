#pragma once

#include <string>
#include <map>

#include <GL/glew.h>

#include "Shader.h"

namespace dummy
{
    namespace common
    {
        class ShaderManager
        {
        public:
            ShaderManager() = default;
            ~ShaderManager();

            void createShader(const std::string& shaderName,
                const std::string& vertexShaderPath,
                const std::string& fragmentShaderPath);

            Shader getShader(const std::string& shaderName) const;

        private:
            std::map<std::string, Shader> m_shaders;
        };
    }
}

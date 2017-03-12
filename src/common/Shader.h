#pragma once

#include <string>

#include <GL/glew.h>

namespace dummy
{
    namespace common
    {
        class Shader
        {
        public:
            Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
            Shader() = default;
            ~Shader() = default;

            void use() const;
            GLuint id() const;

        private:
            std::string readFromFile(const std::string& fileName) const;
            GLuint compile(GLenum type, const std::string& source) const;
            GLuint link(GLuint vertexShader, GLuint fragmentShader) const;

        private:
            GLuint m_id;
        };
    }
}

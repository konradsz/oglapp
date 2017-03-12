#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "Vertex.h"

namespace dummy
{
    namespace rendering
    {
        class Mesh
        {
        public:
            Mesh(const std::vector<Vertex>& vertices);
            void draw();

        private:
            std::vector<Vertex> m_vertices;

            GLuint m_vao;
            GLuint m_vbo;
        };
    }
}

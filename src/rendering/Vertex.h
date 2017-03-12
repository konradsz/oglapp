#pragma once

#include <glm/glm.hpp>

namespace dummy
{
    namespace rendering
    {
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };
    }
}

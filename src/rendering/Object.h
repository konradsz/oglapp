#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Mesh.h"

namespace dummy
{
    namespace rendering
    {
        class Object
        {
        public:
            Object(Mesh mesh);
            Object(Mesh mesh, glm::vec3 position, float scale, bool oneColor);
            ~Object() = default;

            Mesh getMesh() const;
            glm::vec3 getPosition() const;
            float getScale() const;
            glm::mat4 getTransformation() const;
            bool oneColor() const;

        private:
            Mesh m_mesh;

            glm::vec3 m_position;
            float m_scale;
            bool m_oneColor;
        };
    }
}

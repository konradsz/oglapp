#pragma once

#include <memory>
#include <vector>

#include "Renderer.h"
#include "Object.h"

namespace dummy
{
    namespace rendering
    {
        class Scene
        {
        public:
            Scene(std::shared_ptr<Renderer> renderer); // unique_ptr na renderer moze?
            ~Scene() = default;

            void addObject(Object object);
            void draw();

        private:
            std::shared_ptr<Renderer> m_renderer;
            std::vector<Object> m_objects;
        };
    }
}

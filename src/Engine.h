#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "common/ShaderManager.h"
#include "rendering/Renderer.h"
#include "rendering/Scene.h"

namespace dummy
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        void run();

    private:
        void processInput();
        void update(double timeStep);
        void render();

    private:
        GLFWwindow* m_window;

        std::shared_ptr<rendering::Renderer> m_renderer; // unique_ptr moze?
        rendering::Scene m_scene;

    };
}

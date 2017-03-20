#include <memory>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

#include "Engine.h"
#include "common/ShaderManager.h"
#include "common/CameraOperator.h"
#include "common/Camera.h"
#include "rendering/Vertex.h"

const GLuint WIDTH = 800, HEIGHT = 600;

const int UPDATE_RATE = 120;
double timeStep = 1.0f / UPDATE_RATE;

auto camera = std::make_shared<dummy::common::Camera>(glm::vec3(0.0f, 0.0f, 5.0f), 0.0f, -90.0f);
dummy::common::CameraOperator cameraOperator(camera, 4.0f, 0.05f);

GLfloat deltaTime = 0.0f;

bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

    cameraOperator.processMouse(xoffset, yoffset);
}

namespace dummy
{
    Engine::Engine() :
        m_renderer(std::make_shared<rendering::Renderer>(WIDTH, HEIGHT)),
        m_scene(m_renderer)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "oglapp", nullptr, nullptr);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            std::terminate();
        }

        glfwMakeContextCurrent(m_window);
        
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCursorPosCallback(m_window, mouse_callback);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            std::terminate();
        }

        int width;
        int height;
        glfwGetFramebufferSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);

        m_renderer->initShaders();
        m_renderer->loadTextures();

        std::vector<rendering::Vertex> cube = {
            // Positions             // Normals       // Texture Coords
            { { -0.5f, -0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 0.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 1.0f, 1.0f } },
            { { -0.5f,  0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 0.0f, 0.0f } },

            { { -0.5f, -0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 0.0f, 0.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 1.0f, 1.0f } },
            { { -0.5f,  0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 0.0f, 0.0f } },

            { { -0.5f,  0.5f,  0.5f },{ -1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },
            { { -0.5f,  0.5f, -0.5f },{ -1.0f,  0.0f,  0.0f },{ 1.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ -1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f },{ -1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f },{ -1.0f,  0.0f,  0.0f },{ 0.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f },{ -1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },

            { { 0.5f,  0.5f,  0.5f },{ 1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 1.0f,  0.0f,  0.0f },{ 1.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 1.0f,  0.0f,  0.0f },{ 0.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },

            { { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f,  0.0f },{ 0.0f, 1.0f } },
            { { 0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f,  0.0f },{ 1.0f, 1.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f,  0.0f },{ 1.0f, 0.0f } },
            { { 0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f,  0.0f },{ 1.0f, 0.0f } },
            { { -0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f,  0.0f },{ 0.0f, 0.0f } },
            { { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f,  0.0f },{ 0.0f, 1.0f } },

            { { -0.5f,  0.5f, -0.5f },{ 0.0f,  1.0f,  0.0f },{ 0.0f, 1.0f } },
            { { 0.5f,  0.5f, -0.5f },{ 0.0f,  1.0f,  0.0f },{ 1.0f, 1.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 0.0f,  1.0f,  0.0f },{ 1.0f, 0.0f } },
            { { 0.5f,  0.5f,  0.5f },{ 0.0f,  1.0f,  0.0f },{ 1.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f },{ 0.0f,  1.0f,  0.0f },{ 0.0f, 0.0f } },
            { { -0.5f,  0.5f, -0.5f },{ 0.0f,  1.0f,  0.0f },{ 0.0f, 1.0f } }
        };

        m_scene.addObject({ rendering::Mesh(cube), { 0.0f, 0.0f, 0.0f }, 1.0f , false });
        m_scene.addObject({ rendering::Mesh(cube),{ 1.0f, 0.0f, 0.0f }, 1.0f , false });
        m_scene.addObject({ rendering::Mesh(cube),{ -1.0f, 0.0f, 0.0f }, 1.0f , false });
        m_scene.addObject({ rendering::Mesh(cube),{ 0.0f, -1.0f, 1.0f }, 1.0f , false });
        m_scene.addObject({ rendering::Mesh(cube),{ 1.0f, -1.0f, 1.0f }, 1.0f , false });
        m_scene.addObject({ rendering::Mesh(cube),{ -1.0f, -1.0f, 1.0f }, 1.0f , false });

        m_scene.addObject({ rendering::Mesh(cube), { 1.2f, 1.0f, 2.0f }, 0.05f , true });

        m_renderer->setCamera(camera);
    }

    Engine::~Engine()
    {
        glfwTerminate();
    }

    void Engine::run()
    {
        double lastTime = glfwGetTime();
        double lag = 0;

        //while (!m_shouldFinish)
        while (!glfwWindowShouldClose(m_window))
        {
            double deltaTime = glfwGetTime() - lastTime;

            lastTime = glfwGetTime();
            lag += deltaTime;
            //std::cout << deltaTime << " " << lag << " " << timeStep << std::endl;
            glfwPollEvents();
            update(deltaTime);

            while (lag >= timeStep)
            {
                lag -= timeStep;
            }

            render();
        }
    }

    void Engine::processInput()
    {

    }

    void Engine::update(double timeStep)
    {
        GLfloat lightSpeed = 1.0f * timeStep;
        if (keys[GLFW_KEY_W])
            cameraOperator.processKeyboard(dummy::common::Direction::Forward, timeStep);
        if (keys[GLFW_KEY_S])
            cameraOperator.processKeyboard(dummy::common::Direction::Backward, timeStep);
        if (keys[GLFW_KEY_A])
            cameraOperator.processKeyboard(dummy::common::Direction::Left, timeStep);
        if (keys[GLFW_KEY_D])
            cameraOperator.processKeyboard(dummy::common::Direction::Right, timeStep);
        /*if (keys[GLFW_KEY_UP])
            lightPos += lightSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        if (keys[GLFW_KEY_DOWN])
            lightPos += lightSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
        if (keys[GLFW_KEY_LEFT])
            lightPos += lightSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
        if (keys[GLFW_KEY_RIGHT])
            lightPos += lightSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        if (keys[GLFW_KEY_DELETE])
            lightPos += lightSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
        if (keys[GLFW_KEY_PAGE_DOWN])
            lightPos += lightSpeed * glm::vec3(0.0f, 0.0f, -1.0f);*/
    }

    void Engine::render()
    {
        m_scene.draw();

        // Swap the screen buffers
        glfwSwapBuffers(m_window);
    }
}

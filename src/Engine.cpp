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



GLuint diffuseMap;
GLuint specularMap;
GLuint VBO, VAO;

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
        m_renderer(WIDTH, HEIGHT)
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

        m_renderer.initShaders();

        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            // Positions           // Normals           // Texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // Texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);

        glGenTextures(1, &diffuseMap);
        int widthxxx, heightxxx;
        unsigned char* image;
        // Diffuse map
        image = SOIL_load_image("../resources/textures/container2.png", &widthxxx, &heightxxx, 0, SOIL_LOAD_RGB);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthxxx, heightxxx, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Specular map
        glGenTextures(1, &specularMap);
        unsigned char* image2;
        image2 = SOIL_load_image("../resources/textures/container2_specular.png", &widthxxx, &heightxxx, 0, SOIL_LOAD_RGB);
        //image2 = SOIL_load_image("../specular_black.png", &width, &height, 0, SOIL_LOAD_RGB);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthxxx, heightxxx, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

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
        auto mesh = std::make_unique<rendering::Mesh>(cube);
        m_renderer.addMesh(std::move(mesh), "light");
        m_renderer.setCamera(camera);
    }

    Engine::~Engine()
    {
        // Properly de-allocate all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        // Terminate GLFW, clearing any resources allocated by GLFW.
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
                //std::cout << "Elo" << std::endl;
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
        /*auto id = program.getShader("object").id();
        program.getShader("object").use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glUniform1i(glGetUniformLocation(id, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glUniform1i(glGetUniformLocation(id, "material.specular"), 1);
        glUseProgram(id);

        auto camPos = camera->getPosition();
        glUniform3f(glGetUniformLocation(id, "viewPos"), camPos.x, camPos.y, camPos.z);



        glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(glGetUniformLocation(id, "light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(id, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(id, "light.specular"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(id, "light.lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glUniform1f(glGetUniformLocation(id, "material.shininess"), 32.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);*/

        m_renderer.render();

        // Swap the screen buffers
        glfwSwapBuffers(m_window);
    }
}

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Mesh.h"

using namespace dummy::rendering;
using namespace dummy::common;

Renderer::Renderer(int windowWidth, int windowHeight) :
    WINDOW_WIDTH(windowWidth),
    WINDOW_HEIGHT(windowHeight)
{
}

void Renderer::initShaders()
{
    m_shaderManager.createShader("object", "../resources/shaders/vertex.vert", "../resources/shaders/fragment.frag");
    m_shaderManager.createShader("light", "../resources/shaders/vertex.vert", "../resources/shaders/light.frag");
}

void Renderer::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model;
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix();

    for (const auto& mesh : m_meshes)
    {
        auto id = m_shaderManager.getShader(mesh.shader).id();
        glUseProgram(id);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.55f));

        glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        mesh.mesh->draw();
    }
}

void Renderer::addMesh(std::unique_ptr<Mesh> mesh, const std::string& shader)
{
    m_meshes.push_back({ std::move(mesh), shader });
}

void Renderer::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

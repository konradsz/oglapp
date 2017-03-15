#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

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

void Renderer::loadTextures()
{
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
}

void Renderer::clearScene() const
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const Object& object)
{
    glm::mat4 model = object.getTransformation();
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix();

    GLuint id = 0;

    if (object.oneColor())
    {
        id = m_shaderManager.getShader("light").id();
        m_shaderManager.getShader("light").use(); // czy use nie moze zwracac referencji do shadera (chainning)    
    }
    else
    {
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        id = m_shaderManager.getShader("object").id();
        m_shaderManager.getShader("object").use(); // czy use nie moze zwracac referencji do shadera (chainning)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glUniform1i(glGetUniformLocation(id, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glUniform1i(glGetUniformLocation(id, "material.specular"), 1);

        auto camPos = m_camera->getPosition();
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
    }

    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    object.getMesh().draw();
}

void Renderer::addMesh(std::unique_ptr<Mesh> mesh, const std::string& shader)
{
    m_meshes.push_back({ std::move(mesh), shader });
}

void Renderer::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

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
    int width, height;
    unsigned char* image;
    // Diffuse map
    image = SOIL_load_image("../resources/textures/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Specular map
    glGenTextures(1, &specularMap);
    image = SOIL_load_image("../resources/textures/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
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
        auto shader = m_shaderManager.getShader("light");
        id = shader.id();
        shader.use();
    }
    else
    {
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        auto shader = m_shaderManager.getShader("object");
        id = shader.id();
        shader.use();

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

        glUniform3f(glGetUniformLocation(id, "directionalLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "directionalLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(id, "directionalLight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(id, "directionalLight.direction"), -0.2f, -1.0f, -0.3f);

        glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
        };
        glUniform3f(glGetUniformLocation(id, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(id, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(id, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(id, "pointLights[0].quadratic"), 0.032);

        glUniform3f(glGetUniformLocation(id, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(id, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(id, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(id, "pointLights[1].quadratic"), 0.032);

        glUniform3f(glGetUniformLocation(id, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(id, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(id, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(id, "pointLights[2].quadratic"), 0.032);

        glUniform3f(glGetUniformLocation(id, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(id, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(id, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(id, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(id, "pointLights[3].quadratic"), 0.032);

        auto camDir = m_camera->getDirection();
        glUniform3f(glGetUniformLocation(id, "spotLight.position"), camPos.x, camPos.y, camPos.z);
        glUniform3f(glGetUniformLocation(id, "spotLight.direction"), camDir.x, camDir.y, camDir.z);
        glUniform1f(glGetUniformLocation(id, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(id, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
        glUniform1f(glGetUniformLocation(id, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(id, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(id, "spotLight.quadratic"), 0.032);
        glUniform3f(glGetUniformLocation(id, "spotLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(id, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(id, "spotLight.specular"), 1.0f, 1.0f, 1.0f);

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

void Renderer::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace common
{
    class Camera
    {
    public:
        Camera(const glm::vec3& position, GLfloat pitch, GLfloat yaw);

        void translate(const glm::vec3& v);
        void yaw(GLfloat angle);
        void pitch(GLfloat angle);

        glm::mat4 getViewMatrix() const;
        glm::vec3 getPosition() const;
        glm::vec3 getDirection() const;
        glm::vec3 getRight() const;

    private:
        void updateDirection();

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_up;
        glm::vec3 m_right;

        GLfloat m_yaw;
        GLfloat m_pitch;
    };
}

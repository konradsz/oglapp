#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dummy
{
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
            glm::mat4 getProjectionMatrix() const;
            glm::vec3 getPosition() const;
            glm::vec3 getDirection() const;
            glm::vec3 getRight() const;

        private:
            void updateDirection();

        private:
            float m_fov;
            float m_aspectRatio;
            float m_nearPlane;
            float m_farPlane;

            glm::vec3 m_position;
            glm::vec3 m_direction;
            glm::vec3 m_up;
            glm::vec3 m_right;

            GLfloat m_yaw;
            GLfloat m_pitch;
        };
    }
}

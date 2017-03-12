#include "Camera.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using dummy::common::Camera;

Camera::Camera(const glm::vec3& position, GLfloat pitch, GLfloat yaw) :
    m_position(position),
    m_yaw(yaw),
    m_pitch(pitch)
{
    updateDirection();
}

void Camera::translate(const glm::vec3& v)
{
    m_position += v;
}

void Camera::yaw(GLfloat angle)
{
    m_yaw = glm::mod(m_yaw + angle, 360.0f);;
    updateDirection();
}

void Camera::pitch(GLfloat angle)
{
    m_pitch += angle;

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    updateDirection();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

glm::vec3 Camera::getPosition() const
{
    return m_position;
}

glm::vec3 Camera::getDirection() const
{
    return m_direction;
}

glm::vec3 Camera::getRight() const
{
    return m_right;
}

void Camera::updateDirection()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_direction = glm::normalize(direction);
    // we use vec(0.0f, 1.0f, 0.0f) to calculate right vector, because this camera 
    // does not implement roll and we limited pitch value to 89 degrees,
    // so m_direction will never be vec(0.0f, 1.0f, 0.0f), thus cross product gives always correct result
    m_right = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_direction));
}

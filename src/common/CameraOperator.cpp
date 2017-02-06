#include "CameraOperator.h"
#include "Camera.h"

using namespace common;

CameraOperator::CameraOperator(const std::shared_ptr<Camera>& camera, GLfloat movementSpeed, GLfloat mouseSensivity) :
    m_camera(camera),
    m_movementSpeed(movementSpeed),
    m_mouseSensivity(mouseSensivity)
{
}

void CameraOperator::processKeyboard(Direction direction, GLfloat deltaTime)
{
    GLfloat step = m_movementSpeed * deltaTime;

    switch (direction)
    {
    case Direction::Forward:
        m_camera->translate(m_camera->getDirection() * step);
        break;
    case Direction::Backward:
        m_camera->translate(-m_camera->getDirection() * step);
        break;
    case Direction::Left:
    {
        m_camera->translate(-m_camera->getRight() * step);
        break;
    }
    case Direction::Right:
        m_camera->translate(m_camera->getRight() * step);
        break;
    }
}

void CameraOperator::processMouse(GLfloat xOffset, GLfloat yOffset)
{
    m_camera->yaw(xOffset * m_mouseSensivity);
    m_camera->pitch(yOffset * m_mouseSensivity);
}

void CameraOperator::setMovementSpeed(GLfloat speed)
{
    m_movementSpeed = speed;
}

void CameraOperator::setMouseSensivity(GLfloat sensivity)
{
    m_mouseSensivity = sensivity;
}

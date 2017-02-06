#pragma once

#include <memory>
#include <GL/glew.h>

namespace common
{
    enum class Direction
    {
        Forward,
        Backward,
        Left,
        Right
    };

    class Camera;

    class CameraOperator
    {
    public:
        CameraOperator(const std::shared_ptr<Camera>& camera, GLfloat movementSpeed, GLfloat mouseSensivity);

        void processKeyboard(Direction direction, GLfloat deltaTime);
        void processMouse(GLfloat xOffset, GLfloat yOffset);

        void setMovementSpeed(GLfloat speed);
        void setMouseSensivity(GLfloat sensivity);

    private:
        std::shared_ptr<Camera> m_camera;
        GLfloat m_movementSpeed;
        GLfloat m_mouseSensivity;
    };
}

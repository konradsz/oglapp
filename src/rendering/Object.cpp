#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"

using namespace dummy::rendering;

Object::Object(Mesh mesh) :
    m_mesh(mesh),
    m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f),
    m_oneColor(false)
{
}

Object::Object(Mesh mesh, glm::vec3 position, float scale, bool oneColor) :
    m_mesh(mesh),
    m_position(position),
    m_scale(scale),
    m_oneColor(oneColor)
{
}

Mesh Object::getMesh() const
{
    return m_mesh;
}

glm::vec3 Object::getPosition() const
{
    return m_position;
}

float Object::getScale() const
{
    return m_scale;
}

glm::mat4 Object::getTransformation() const
{
    auto transformation = glm::mat4();
    transformation = glm::translate(transformation, m_position);
    transformation = glm::scale(transformation, glm::vec3(m_scale));

    return transformation;
}

bool Object::oneColor() const
{
    return m_oneColor;
}

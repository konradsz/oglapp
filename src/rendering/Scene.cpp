#include "Scene.h"

using namespace dummy::rendering;

Scene::Scene(std::shared_ptr<Renderer> renderer) :
    m_renderer(renderer)
{
}

void Scene::addObject(Object object)
{
    m_objects.push_back(object);
}

void Scene::draw()
{
    m_renderer->clearScene();
    for (auto& object : m_objects)
    {
        m_renderer->render(object);
    }
}

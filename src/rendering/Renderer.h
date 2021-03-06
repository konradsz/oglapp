#pragma once

#include <vector>
#include <memory>

#include "../common/ShaderManager.h"
#include "../common/Camera.h"
#include "Object.h"
#include "Mesh.h"

namespace dummy
{
    namespace rendering
    {
        struct BundledMesh
        {
            std::unique_ptr<Mesh> mesh;
            std::string shader;
        };

        class Renderer
        {
        public:
            Renderer(int windowWidth, int windowHeight);
            void initShaders();
            void loadTextures();

            void clearScene() const;
            void render(const Object& object);

            //TU POWINIEN BYC WEAK_PTR
            void setCamera(std::shared_ptr<common::Camera> camera);

        private:
            const int WINDOW_WIDTH;
            const int WINDOW_HEIGHT;

            common::ShaderManager m_shaderManager;
            //TU POWINIEN BYC WEAK_PTR
            std::shared_ptr<common::Camera> m_camera;
            //std::vector<std::unique_ptr<Mesh>> m_meshes;
            std::vector<BundledMesh> m_meshes;

            GLuint diffuseMap;
            GLuint specularMap;
        };
    }
}
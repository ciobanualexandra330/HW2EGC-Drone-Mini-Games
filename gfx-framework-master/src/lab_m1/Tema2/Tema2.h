#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include <string>
#include <unordered_map>
#include "components/transform.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cstdlib> 

using namespace std;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        std::unordered_map<std::string, Texture2D*> mapTextures;

        void RenderScene();


    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float dronex, droney, dronez;
        glm::vec3 dronePosition;

        glm::vec3 cameraTargetPosition;
        glm::vec3 cameraOffset;

        float rotorAngle = 0.0f;
        float rotorSpeed = 180.0f;
        std::vector<float> treeX;
        std::vector<float> treeY;
        std::vector<float> treeZ;

        ViewportArea miniViewportArea;
        
        Mesh* Tema2::GenerateGridMesh(int rows, int cols, float cellSize);

        Mesh* Tema2::CreateCylinder(float radius, float height, unsigned int segments, glm::vec3 color);

        Mesh* CreateParallelepiped(const std::string& name, glm::vec3 size);

        Mesh* CreateConus(float radius, float height, unsigned int segments, glm::vec3 color);

    };
}   // namespace m1

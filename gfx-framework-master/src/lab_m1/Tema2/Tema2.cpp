#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2() 
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    cameraTargetPosition = glm::vec3(0, 3, 0);
    cameraOffset = glm::vec3(0, 10, -15);

    Mesh* terrainMesh = GenerateGridMesh(150, 150, 0.5f);
    meshes["terrain"] = terrainMesh;

    dronex = -6.0f;
    droney = 3;
    dronez = 1;

    treeX.resize(10);
    treeZ.resize(10);

    Shader* shader = new Shader("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    Shader* grayShader = new Shader("Gray");
    grayShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "gray_vertex.glsl"), GL_VERTEX_SHADER);
    grayShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "gray_fragment.glsl"), GL_FRAGMENT_SHADER);
    grayShader->CreateAndLink();
    shaders[grayShader->GetName()] = grayShader;

    Shader* maroShader = new Shader("MaroShader");
    maroShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "maro_vertex.glsl"), GL_VERTEX_SHADER);
    maroShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "maro_fragment.glsl"), GL_FRAGMENT_SHADER);
    maroShader->CreateAndLink();
    shaders[maroShader->GetName()] = maroShader;

    Shader* textureShader = new Shader("Texture");
    textureShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "texture_vertex.glsl"), GL_VERTEX_SHADER);
    textureShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "texture_fragment.glsl"), GL_FRAGMENT_SHADER);
    textureShader->CreateAndLink();
    shaders[textureShader->GetName()] = textureShader;

    Shader* greenShader = new Shader("Green");
    greenShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "green_vertex.glsl"), GL_VERTEX_SHADER);
    greenShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "green_fragment.glsl"), GL_FRAGMENT_SHADER);
    greenShader->CreateAndLink();
    shaders[greenShader->GetName()] = greenShader;


    Mesh* tree = CreateConus(1.0f, 2.0f, 32, glm::vec3(0.6f, 0.3f, 0.1f));
    meshes["con"] = tree;

    Mesh* cylinder = CreateCylinder(1.0f, 2.0f, 32, glm::vec3(0.6f, 0.3f, 0.1f));
    meshes["cylinder"] = cylinder;

    Mesh* cubeA = CreateParallelepiped("cube", glm::vec3(1.0f, 1.0f, 1.0f));
    meshes["cube"] = cubeA;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < 10; i++) {
        bool validPosition = false;
        while (!validPosition) {
            float randomX = -30.0f + static_cast<float>(rand()) / RAND_MAX * (30.0f - (-30.0f));
            float randomZ = -30.0f + static_cast<float>(rand()) / RAND_MAX * (30.0f - (-30.0f));

            validPosition = true;
            for (int j = 0; j < i; j++) {
                float dx = randomX - treeX[j];
                float dz = randomZ - treeZ[j];
                float distance = sqrt(dx * dx + dz * dz);

                if (distance < 6.0f) {
                    validPosition = false;
                    break;
                }
            }
            float dxDrone = randomX - dronex;
            float dzDrone = randomZ - dronez;
            float distanceToDrone = sqrt(dxDrone * dxDrone + dzDrone * dzDrone);

            if (distanceToDrone < 8.0f) {
                validPosition = false;
            }
            if (validPosition) {
                treeX[i] = randomX;
                treeZ[i] = randomZ;
            }
        }
    }

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

Mesh* Tema2::GenerateGridMesh(int rows, int cols, float cellSize) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float halfWidth = cols * cellSize / 2.0f;
    float halfHeight = rows * cellSize / 2.0f;

    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= cols; j++) {
            float x = -halfWidth + j * cellSize;
            float z = -halfHeight + i * cellSize;
            vertices.emplace_back(glm::vec3(x, 0, z), glm::vec3(0, 1, 0), glm::vec3(0.6f, 0.4f, 0.2f));
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int topLeft = i * (cols + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (cols + 1) + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh* mesh = new Mesh("terrain");
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* Tema2::CreateConus(float radius, float height, unsigned int segments, glm::vec3 color) {
    color = glm::vec3(0.5f, 0.5f, 0.5f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), color);
    for (unsigned int i = 0; i < segments; i++) {
        float angle = 2.0f * glm::pi<float>() * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, 0, z), glm::vec3(0, -1, 0), color);
    }

    vertices.emplace_back(glm::vec3(0, height, 0), glm::vec3(0, 1, 0), color);

    for (unsigned int i = 1; i <= segments; i++) {
        unsigned int next = (i % segments) + 1;
        indices.push_back(0);
        indices.push_back(next);
        indices.push_back(i);
    }

    unsigned int topIndex = segments + 1;
    for (unsigned int i = 1; i <= segments; i++) {
        unsigned int next = (i % segments) + 1;
        indices.push_back(i);
        indices.push_back(next);
        indices.push_back(topIndex);
    }

    Mesh* con = new Mesh("con");
    con->InitFromData(vertices, indices);
    return con;
}

Mesh* Tema2::CreateCylinder(float radius, float height, unsigned int segments, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    color = glm::vec3(0.6f, 0.3f, 0.1f);

    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), color);
    for (unsigned int i = 0; i < segments; i++) {
        float angle = 2.0f * glm::pi<float>() * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, 0, z), glm::vec3(0, -1, 0), color);
    }

    unsigned int offset = segments + 1;
    vertices.emplace_back(glm::vec3(0, height, 0), glm::vec3(0, 1, 0), color);
    for (unsigned int i = 0; i < segments; i++) {
        float angle = 2.0f * glm::pi<float>() * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, height, z), glm::vec3(0, 1, 0), color);
    }

    for (unsigned int i = 1; i <= segments; i++) {
        unsigned int next = (i % segments) + 1;
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(next);
    }

    unsigned int topCenterIndex = offset;
    for (unsigned int i = 1; i <= segments; i++) {
        unsigned int next = (i % segments) + 1;
        indices.push_back(topCenterIndex);
        indices.push_back(offset + next);
        indices.push_back(offset + i);
    }

    for (unsigned int i = 1; i <= segments; i++) {
        unsigned int next = (i % segments) + 1;

        indices.push_back(i);
        indices.push_back(next);
        indices.push_back(offset + i);

        indices.push_back(next);
        indices.push_back(offset + next);
        indices.push_back(offset + i);
    }

    Mesh* cylinder = new Mesh("cylinder");
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}

Mesh* Tema2::CreateParallelepiped(const std::string& name, glm::vec3 size) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2)),
        VertexFormat(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2)),
        VertexFormat(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2)),
        VertexFormat(glm::vec3(size.x / 2, size.y / 2, -size.z / 2)),
        VertexFormat(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2)),
        VertexFormat(glm::vec3(size.x / 2, -size.y / 2, size.z / 2)),
        VertexFormat(glm::vec3(-size.x / 2, size.y / 2, size.z / 2)),
        VertexFormat(glm::vec3(size.x / 2, size.y / 2, size.z / 2)),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 1, 3, 2, 
        4, 5, 6, 5, 7, 6, 
        0, 2, 4, 2, 6, 4, 
        1, 3, 5, 3, 7, 5, 
        2, 3, 6, 3, 7, 6, 
        0, 1, 4, 1, 5, 4 
    };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

void Tema2::FrameStart()
{
    glClearColor(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema2::RenderScene() {
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["terrain"], shaders["LabShader"], modelMatrix);

    // obstacole ----------------------------------------------------------------------
    for (float i = 0; i < 10; i++) {
        if (i < 7) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(treeX[i], 3, treeZ[i]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 5, 3));
            RenderMesh(meshes["con"], shaders["Green"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(treeX[i], 0, treeZ[i]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 3.0f, 0.8f));
            RenderMesh(meshes["cylinder"], shaders["MaroShader"], modelMatrix);
        }
        else if (i >= 7) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(treeX[i], 3, treeZ[i]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 12, 5));
            RenderMesh(meshes["cube"], shaders["Texture"], modelMatrix);
        }
    }
    // drone ------------------------------------------------------------------------
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney, dronez));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 8));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney, dronez));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(8, 1, 1));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex - 3.5f, droney + 1, dronez));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex + 3.5f, droney + 1, dronez));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney + 1, dronez + 3.5f));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney + 1, dronez - 3.5f));
    RenderMesh(meshes["cube"], shaders["Gray"], modelMatrix);

    // rotor -----------------------------------------------------------------
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex - 3.5f, droney + 1.6f, dronez));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 0.2, 0.2));
    RenderMesh(meshes["cube"], shaders["MaroShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex + 3.5f, droney + 1.6f, dronez));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 0.2, 0.2));
    RenderMesh(meshes["cube"], shaders["MaroShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney + 1.6f, dronez + 3.5f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 0.2, 0.2));
    RenderMesh(meshes["cube"], shaders["MaroShader"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronex, droney + 1.6f, dronez - 3.5f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 0.2, 0.2));
    RenderMesh(meshes["cube"], shaders["MaroShader"], modelMatrix);
}

void Tema2::Update(float deltaTimeSeconds) {

    float droneRotationAngle = glm::radians(45.0f);

    glm::vec3 droneForward = glm::normalize(glm::vec3(0, 0, -1));
    glm::vec3 direction = glm::vec3(glm::rotate(glm::mat4(1.0f), droneRotationAngle, glm::vec3(0, 1, 0)) * glm::vec4(droneForward, 1.0));

    glm::vec3 dronePosition = glm::vec3(dronex, droney, dronez);

    glm::vec3 cameraPosition = dronePosition + cameraOffset;

    camera->Set(cameraPosition, dronePosition, glm::vec3(0, 1, 0));

    rotorAngle += rotorSpeed * deltaTimeSeconds;
    if (rotorAngle > 360.0f) {
        rotorAngle -= 360.0f;
    }
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene();
    glm::vec3 savedPosition = camera->position;
    glm::vec3 savedForward = camera->forward;
    glm::vec3 savedRight = camera->right;
    glm::vec3 savedUp = camera->up;

    glClear(GL_DEPTH_BUFFER_BIT);
    miniViewportArea = ViewportArea(-100, 0, resolution.x / 2.5f, resolution.y / 2.5f);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    camera->Set(glm::vec3(0, 80, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    RenderScene();
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    GLint modelLoc = glGetUniformLocation(shader->program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewLoc = glGetUniformLocation(shader->program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    GLint projLoc = glGetUniformLocation(shader->program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint freqLoc = glGetUniformLocation(shader->program, "frequency");
    glUniform1f(freqLoc, 0.2f);

    GLint freq = glGetUniformLocation(shader->program, "freq");
    glUniform1f(freq, 1);

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 10.0f;
    float rotationSpeed = 20;

    if (window->KeyHold(GLFW_KEY_W)) {
        float newZ = dronez + speed * deltaTime;
        bool collision = false;

        if (droney < 13.0f) {
            for (int i = 0; i < treeX.size(); i++) {
                float dx = dronex - treeX[i];
                float dz = newZ - treeZ[i];
                float distance = sqrt(dx * dx + dz * dz);
                if (distance < 7.0f) {
                    collision = true;
                    break;
                }
            }
        }
        if (!collision) {
            dronez = newZ;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        float newZ = dronez - speed * deltaTime;
        bool collision = false;

        if (droney < 13.0f) {
            for (int i = 0; i < treeX.size(); i++) {
                float dx = dronex - treeX[i];
                float dz = newZ - treeZ[i];
                float distance = sqrt(dx * dx + dz * dz);
                if (distance < 7.0f) {
                    collision = true;
                    break;
                }
            }
        }
        if (!collision) {
            dronez = newZ;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        float newX = dronex + speed * deltaTime;
        bool collision = false;

        if (droney < 13.0f) {
            for (int i = 0; i < treeX.size(); i++) {
                float dx = newX - treeX[i];
                float dz = dronez - treeZ[i];
                float distance = sqrt(dx * dx + dz * dz);
                if (distance < 7.0f) {
                    collision = true;
                    break;
                }
            }
        }
        if (!collision) {
            dronex = newX;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        float newX = dronex - speed * deltaTime;
        bool collision = false;
        if (droney < 13.0f) {
            for (int i = 0; i < treeX.size(); i++) {
                float dx = newX - treeX[i];
                float dz = dronez - treeZ[i];
                float distance = sqrt(dx * dx + dz * dz);
                if (distance < 7.0f) {
                    collision = true;
                    break;
                }
            }
        }
        if (!collision) {
            dronex = newX;
        }
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        droney += speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        float newY = droney - speed * deltaTime;
        bool aboveTree = false;
        for (int i = 0; i < treeX.size(); i++) {
            float dx = dronex - treeX[i];
            float dz = dronez - treeZ[i];
            float distance = sqrt(dx * dx + dz * dz);

            if (distance < 7.0f && droney <= 13.0f) {
                aboveTree = true;
                break;
            }
        }
        if (!aboveTree && newY > 1) {
            droney = newY;
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }

}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
            camera->RotateFirstPerson_OX(-sensivityOY * deltaY);

        }
    
        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
            camera->RotateThirdPerson_OX(-sensivityOY * deltaY);

        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

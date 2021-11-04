#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();

    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;
    
    Mesh* obstacle1 = object2D::CreateSquare1("obstacle1", glm::vec3(0.2, 0.2, 0), 0.19f, 0.30f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = object2D::CreateSquare1("obstacle2", glm::vec3(0.1, 0.9, 0), 0.4f, 0.25f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = object2D::CreateSquare1("obstacle3", glm::vec3(0.4, 0.6, 0), 0.3f, 0.1f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle3);

    Mesh* obstacle4 = object2D::CreateSquare1("obstacle4", glm::vec3(1, 1, 0), 0.2f, 0.4f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle4);

    Mesh* obstacle5 = object2D::CreateSquare1("obstacle5", glm::vec3(0.8, 0.7, 0), 0.7f, 0.2f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle5);

    Mesh* projectile = object2D::CreateSquare1("projectile", glm::vec3(0.7, 0.7, 0), 0.03f, 0.06f, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile);

    Mesh* enemy1 = object2D::CreateEnemy("enemy1", glm::vec3(0.3, 0.3, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
    AddMeshToList(enemy1);

    Mesh* circle = object2D::CreateCircle("circle", glm::vec3(0.4, 0.4, 0), 2, glm::vec3(0.7, 0.3, 0));
    AddMeshToList(circle);

    player.angle = 0;
    Mesh* player = object2D::CreatePlayer("player");
    AddMeshToList(player);
}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.63, 0.54, 1.51), true);

    // Compute the 2D visualization matrix
    visMatrix_outside = glm::mat3(1);
    visMatrix_outside *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix_outside);

    // TODO OK???? 
    viewSpace = ViewportSpace(20, 20, resolution.x - 40, resolution.y - 40);  // dimensiunile in care ma pot misca
    SetViewportArea(viewSpace, glm::vec3(0.63, 0.54, 0.51), true);

    // Compute the 2D visualization matrix
    visMatrix_inside = glm::mat3(1);
    visMatrix_inside *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix_inside);
}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix); // TODO de aici eroare ciudata de compilare

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle5"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0) * transform2D::Scale(0.2, 0.2);
    RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0.5, 0.5) * transform2D::Scale(0.02, 0.03);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0.5, 0.7) * transform2D::Scale(0.05, 0.06);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)
    if (window->KeyHold(GLFW_KEY_W)) {
        logicSpace.y -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        logicSpace.x += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        logicSpace.y += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        logicSpace.x -= deltaTime;
    }
    // TODO(student): Zoom in and zoom out logic window with Z and X
    if (window->KeyHold(GLFW_KEY_Z)) {
        logicSpace.height += deltaTime;
        logicSpace.width += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_X)) {
        logicSpace.height -= deltaTime;
        logicSpace.width -= deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

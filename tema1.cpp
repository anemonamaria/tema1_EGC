#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>
#include <math.h>


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

    player.lives = 1.0f;
    player.angle = projectile.angle = 0;
    player.x = projectile.x =  0;
    player.y = projectile.y = 0;
    projectile.length = 30;
    projectile.isCharging = projectile.isMoving = false;
    obstacle.x = obstacle.y = 0;
    

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

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(player.x - 100 / 2.0, player.x + 100 / 2.0, player.y - 100 / 2.0, player.y + 100 / 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
    Mesh* healthBar = object2D::CreateSquare1("healthBar", glm::vec3(3.25, 3.8, 0), 0.7f, 0.10f, glm::vec3(0.850, 0.792, 0.701), true);
    AddMeshToList(healthBar);

    Mesh* healthBar_wf = object2D::CreateSquare1("healthBar_wf", glm::vec3(3.25, 3.8, 0), 0.7f, 0.10f, glm::vec3(0.850, 0.792, 0.710), false);
    AddMeshToList(healthBar_wf);

    Mesh* obstacleMesh = object2D::CreateSquare1("obstacle", glm::vec3(obstacle.x, obstacle.y, 0), 0.5f, 0.5f, glm::vec3(0.529, 0.658, 0.643), true);
    AddMeshToList(obstacleMesh);

    Mesh* margin1 = object2D::CreateSquare1("margin1", glm::vec3(0, 0, 0), 0.05f, 4.f, glm::vec3(0.529, 0.658, 0.643), true);
    AddMeshToList(margin1);
    
    Mesh* margin2 = object2D::CreateSquare1("margin2", glm::vec3(0.f, 3.93f, 0), 4.f, 0.07f, glm::vec3(0.529, 0.658, 0.643), true);
    AddMeshToList(margin2);
    
    Mesh* margin3 = object2D::CreateSquare1("margin3", glm::vec3(3.95f, 0, 0), 0.05f, 4.f, glm::vec3(0.529, 0.658, 0.643), true);
    AddMeshToList(margin3);
    
    Mesh* margin4 = object2D::CreateSquare1("margin4", glm::vec3(0.f, 0.f, 0), 4.f, 0.07f, glm::vec3(0.529, 0.658, 0.643), true);
    AddMeshToList(margin4);

    Mesh* projectile = object2D::CreateSquare1("projectile", glm::vec3(0.0, 0.0, 0), 0.03f, 0.06f, glm::vec3(0.137, 0.011, 0.207), true);
    AddMeshToList(projectile);

    player.angle = 0;
    Mesh* player = object2D::CreatePlayer("player");
    AddMeshToList(player);

    

    {
        numberOfEnemies = 5;
        Mesh* enemy_new;
        for (int i = 0; i < numberOfEnemies; ++i) {
            enemy_aux.width = 0.3f;
            enemy_aux.height = 0.3f;
            srand(time(NULL));
            enemy_aux.x = i * rand() % 4;// / 5; // * 0.1f;
            enemy_aux.y = 0; // rand() % 4 * i / 5; // * 0.1f;
            enemy_aux.diffX = 4.0f - enemy_aux.x;
            enemy_aux.diffY = 4.0f - enemy_aux.y;
            printf("%f diff x %f diff y %f x %f y \n ", enemy_aux.diffX, enemy_aux.diffY, enemy_aux.x, enemy_aux.y);

            enemy_aux.onScreen = true;
            enemy_aux.color = glm::vec3(0.596, 0.427, 0.556);
            enemy_aux.scale = 1;
            enemy.push_back(enemy_aux);

            enemy_new = object2D::CreateEnemy("enemy" + to_string(i), glm::vec3(enemy_aux.x, enemy_aux.y, 0), enemy_aux.color, glm::vec3(0,0,0));
            AddMeshToList(enemy_new);
        }
    }
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

bool Tema1::projectileOutOfBounds() {
    return (projectile.x > 2 || projectile.y < -2 || projectile.y > 2 || projectile.x < -2);
}

bool Tema1::positionOutOfBonds(float x, float y) {
    return (x > 4 || y < 0  || y > 4 || x < 0);
}

void Tema1::ResetProjectile() {
    projectile.x = player.x;
    projectile.y = player.y;
    projectile.angle = player.angle + 3 * M_PI / 2;
    projectile.isMoving = false;
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.937, 0.890, 0.815), true);

    // Compute the 2D visualization matrix
    visMatrix_outside = glm::mat3(1);
    visMatrix_outside *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix_outside);

    // TODO OK???? 
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);  // dimensiunile in care ma pot misca
    SetViewportArea(viewSpace, glm::vec3(0.937, 0.890, 0.815), true);

    // Compute the 2D visualization matrix
    visMatrix_inside = glm::mat3(1);
    /*logicSpace.height = player.y + 500;
    logicSpace.width = player.x + 500;
    logicSpace.x = player.x - 500;
    logicSpace.y = player.y - 500;*/
    visMatrix_inside *= VisualizationTransf2D(logicSpace, viewSpace);
    
    //projectile movement
    {
        if (projectile.isMoving) {
            projectile.x += projectile.power * deltaTimeSeconds * 2.f * cos(projectile.angle);
            projectile.y += projectile.power * deltaTimeSeconds * 2.f * sin(projectile.angle);
        }
        if (projectileOutOfBounds())
            ResetProjectile();
    }
    srand(time(NULL));

    // Enemy Movement
    {  
        for (int i = 0; i < numberOfEnemies; ++i) {
            if (player.y + 2.0f > enemy[i].y) {
                do { 
                    enemy[i].y += deltaTimeSeconds * ( i * 0.05f + 0.2f);
                } while (enemy[i].y + enemy[i].diffY - 2.0f == player.y + 2.0f);
            }
            if(player.y + 2.0f < enemy[i].y) {
                do {
                    enemy[i].y -= deltaTimeSeconds * (i * 0.05f + 0.2f);
                } while (enemy[i].y + enemy[i].diffY - 2.0f == player.y + 2.0f);
            }

            if (player.x + 2.0f > enemy[i].x) {
                do {
                    enemy[i].x += deltaTimeSeconds * (i * 0.05f + 0.2f);
                } while (enemy[i].x + enemy[i].diffX - 2.0f == player.y + 2.0f);
            }
            if(player.x + 2.0f < enemy[i].x) {
                do { // TODO nu se duce bine spre stanga
                    enemy[i].x -= deltaTimeSeconds * (i * 0.05f + 0.2f);
                } while (enemy[i].x + enemy[i].diffX - 2.0f == player.x + 2.0f);
            }
        }
    }
    //printf("en.y %f en.x %f plx %f ply %f \n", enemy[0].y, enemy[0].x, player.x, player.y);


    DrawScene(visMatrix_inside);
}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0) * transform2D::Scale(1.0f, 1.0f);
    RenderMesh2D(meshes["healthBar_wf"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(0, 0) * transform2D::Scale(player.lives, 1.0f); // TODO nu se scaleaza bine
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["margin1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["margin2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["margin3"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["margin4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(player.x + 2.0f, player.y + 2.0f) * transform2D::Scale(0.05, 0.06)
        * transform2D::Rotate(player.angle);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(projectile.x + 2.0f, projectile.y + 2.0f)
        * transform2D::Rotate(projectile.angle);
    RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

    // Enemy Render
    {
        for (int i = 0; i < numberOfEnemies; ++i) {
            modelMatrix = visMatrix_inside * transform2D::Translate(enemy[i].x, enemy[i].y) * transform2D::Scale(0.3f, 0.3f);
            RenderMesh2D(meshes["enemy" + to_string(i)], shaders["VertexColor"], modelMatrix);

        }
    }

    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 0.25f, obstacle.y + 0.4f) * transform2D::Scale( 0.5f, 1.3f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 0.5f, obstacle.y + 2.5f) * transform2D::Scale(0.8f, 0.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 0.5f, obstacle.y + 2.5f) * transform2D::Scale(0.8f, 0.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 0.8f, obstacle.y + 1.f) * transform2D::Scale(0.6f, 0.6f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 1.3f, obstacle.y + 2.4f) * transform2D::Scale(0.6f, 1.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 2.5f, obstacle.y + 3.f) * transform2D::Scale(0.5f, 0.8f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 2.2f, obstacle.y + 1.4f) * transform2D::Scale(0.9f, 0.9f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 3.3f, obstacle.y + 0.7f) * transform2D::Scale(0.5f, 1.4f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(obstacle.x + 3.3f, obstacle.y + 2.7f) * transform2D::Scale(0.5f, 1.4f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);   
    
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)

    glm::ivec2 resolution = window->GetResolution();
    if (window->KeyHold(GLFW_KEY_W)) {
        if (player.y < 1.9f) { 
            player.y += deltaTime;
            if (!projectile.isMoving)
                projectile.y = player.y;

        }
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        if (player.x > -1.9f) {
            player.x -= deltaTime;
            if (!projectile.isMoving)
                projectile.x = player.x;

        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (player.y > -1.9f) {
            player.y -= deltaTime;
            if (!projectile.isMoving)
                projectile.y = player.y;

        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {

        if (player.x < 1.9f){ 
            player.x += deltaTime;
            if (!projectile.isMoving)
                projectile.x = player.x;

        }
    }
    //// TODO(student): Zoom in and zoom out logic window with Z and X
    //if (window->KeyHold(GLFW_KEY_Z)) {
    //    logicSpace.height += deltaTime;
    //    logicSpace.width += deltaTime;
    //    logicSpace.x += deltaTime / 2;  // todo nu merg astea, se scaleaza din colt
    //    logicSpace.y += deltaTime / 2;  // cu astea se poate face logicspace-ul centrat pe jucator ?? la wasd

    //}
    //if (window->KeyHold(GLFW_KEY_X)) {
    //    logicSpace.height -= deltaTime;
    //    logicSpace.width -= deltaTime;
    //    logicSpace.x -= deltaTime / 2;
    //    logicSpace.y -= deltaTime / 2;
    //}

    if (!projectile.isMoving && window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {  
        projectile.isMoving = true;
        projectile.power = 1.1;
    }
    else if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (projectile.isCharging) { // move the arrow
            projectile.isCharging = false;
            projectile.isMoving = true;
            projectile.power = 1.1;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::setPlayerAngle() {
    glm::ivec2 resolution = window->GetResolution();

    int ax = player.x * resolution.x / 4 + resolution.x / 2;
    int ay = player.y * resolution.y / 4 + resolution.y / 2;
    // TODO dupa ce faci viewport ul ok (centrarea) verifica orientarea
    ax = cursorX - ax;
    ay = - cursorY + ay;
    glm::vec2 auxvect = glm::normalize(glm::vec2(ax, ay));
    if (auxvect.y < 0) {
        auxvect.x = -auxvect.x;
    } 
    if(auxvect.x == 0 && auxvect.y < 0) {
        player.angle = 180.f;
    }
    else if (auxvect.x == 0 && auxvect.y > 0) {
        player.angle = 0.f;
    }
    else 
    {
        player.angle = atan(auxvect.x / auxvect.y);
    }
    if (auxvect.y > 0) {
        player.angle = -player.angle + M_PI;
    }
    if (!projectile.isMoving)
        projectile.angle = player.angle + 3 * M_PI / 2;
    
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    // add mouse move event
    cursorX = mouseX - deltaX;
    cursorY = mouseY - deltaY;
    setPlayerAngle();
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

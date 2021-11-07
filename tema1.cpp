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

    player.angle = projectile.angle = 0;
    player.x = projectile.x =  0;
    player.y = projectile.y = 0;
    projectile.length = 30;
    projectile.isCharging = projectile.isMoving = false;

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

    Mesh* obstacle1 = object2D::CreateSquare1("obstacle1", glm::vec3(0.25, 0.4, 0), 0.1f, 0.70f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = object2D::CreateSquare1("obstacle2", glm::vec3(0.5, 2.5, 0), 0.8f, 0.25f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = object2D::CreateSquare1("obstacle3", glm::vec3(0.4, 1.5, 0), 0.3f, 0.3f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle3);

    Mesh* obstacle4 = object2D::CreateSquare1("obstacle4", glm::vec3(1.4, 1, 0), 0.6f, 0.4f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle4);

    Mesh* obstacle5 = object2D::CreateSquare1("obstacle5", glm::vec3(1.7, 2.4, 0), 0.2f, 1.0f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle5);

    Mesh* obstacle6 = object2D::CreateSquare1("obstacle6", glm::vec3(2.5, 3, 0), 0.5f, 0.50f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle6);
    
    Mesh* obstacle7 = object2D::CreateSquare1("obstacle7", glm::vec3(2.7, 1.4, 0), 0.6f, 0.3f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle7);
    
    Mesh* obstacle8 = object2D::CreateSquare1("obstacle8", glm::vec3(3.7, 0.7, 0), 0.1f, 1.0f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle8);

    Mesh* obstacle9 = object2D::CreateSquare1("obstacle9", glm::vec3(3.7, 2.7, 0), 0.1f, 1.0f, glm::vec3(0.5, 0.5, 0), true);
    AddMeshToList(obstacle9);

    Mesh* projectile = object2D::CreateSquare1("projectile", glm::vec3(0.0, 0.0, 0), 0.03f, 0.06f, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile);

    //Mesh* enemy1 = object2D::CreateEnemy("enemy1", glm::vec3(0.3, 0.3, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
    //AddMeshToList(enemy1);

    Mesh* circle = object2D::CreateCircle("circle", glm::vec3(0.4, 0.4, 0), 2, glm::vec3(0.7, 0.3, 0));
    AddMeshToList(circle);

    player.angle = 0;
    Mesh* player = object2D::CreatePlayer("player");
    AddMeshToList(player);

    srand(time(NULL));

    {
        numberOfEnemies = 5;
        Mesh* enemy_new;
        for (int i = 0; i < numberOfEnemies; ++i) {
            enemy_aux.width = 0.3f;
            enemy_aux.height = 0.3f;
            enemy_aux.x =  i * rand() % 4;
            enemy_aux.y = enemy_aux.height - rand() % 4 * i;
            if (i >= numberOfEnemies / 2 + 1)
                enemy_aux.y -= 0.5;

            enemy_aux.onScreen = true;
            enemy_aux.color = (i < numberOfEnemies / 2 + 1) ? glm::vec3(1, 0, 0) : glm::vec3(1, 1, 0);
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
    projectile.angle = player.angle;
    projectile.isMoving = false;
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
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);  // dimensiunile in care ma pot misca
    SetViewportArea(viewSpace, glm::vec3(0.63, 0.54, 0.51), true);

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
            /*if (window->KeyHold(GLFW_KEY_D) || window->KeyHold(GLFW_KEY_W)
                || window->KeyHold(GLFW_KEY_S) || window->KeyHold(GLFW_KEY_A)) {
                enemy[i].y += deltaTimeSeconds * 0.35f * player.y;
                enemy[i].x += deltaTimeSeconds * 0.35f * player.x;
            }
            else {*/
            if (player.y + 2.0f > enemy[i].y) {
                do {
                    enemy[i].y += deltaTimeSeconds * 0.3f;
                } while (enemy[i].y == player.y + 2.0f);
            }
            else {
                do {
                    enemy[i].y -= deltaTimeSeconds * 0.3f;
                } while (enemy[i].y == player.y + 2.0f);
            }
            if (player.x + 2.0f > enemy[i].x) {
                do {
                    enemy[i].x += deltaTimeSeconds * 0.3f;
                } while (enemy[i].x == player.y + 2.0f);
            }
            else {
                do {
                    enemy[i].x -= deltaTimeSeconds * 0.3f;
                } while (enemy[i].x == player.x + 2.0f);
            }
                
                /*do{
                    enemy[i].x += deltaTimeSeconds * 0.2f;
                } while (enemy[i].x == player.x);*/
                
                
            //}
             
            if (positionOutOfBonds(enemy[i].x, enemy[i].y)) {
                enemy[i].x = rand() % 4 * i;
                enemy[i].y = enemy[i].height - rand() % 4 * i;
            }
        }
    }


    

    DrawScene(visMatrix_inside);
}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix); // TODO de aici eroare ciudata de compilare

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle5"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle6"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle7"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle8"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["obstacle9"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(projectile.x + 2.0f, projectile.y + 2.0f) 
       * transform2D::Rotate(projectile.angle);
    RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

    /* modelMatrix = visMatrix_outside * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0, 0) * transform2D::Scale(0.2, 0.2);
    RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_outside * transform2D::Translate(0.5, 0.5) * transform2D::Scale(0.02, 0.03);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    */
    modelMatrix = visMatrix_inside * transform2D::Translate(player.x + 2.0f, player.y + 2.0f) * transform2D::Scale(0.05, 0.06)
        * transform2D::Rotate(player.angle);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);
     
    // Enemy Render
    {
        
        for (int i = 0; i < numberOfEnemies; ++i) {
            enemy[i].angle = -player.angle; // atan(player.y / player.x);
            modelMatrix = visMatrix_inside * transform2D::Translate(enemy[i].x, enemy[i].y) * transform2D::Scale(0.3f, 0.3f)
                ;// *transform2D::Rotate(enemy[i].angle);
            RenderMesh2D(meshes["enemy" + to_string(i)], shaders["VertexColor"], modelMatrix);
            //}
        }
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

bool Tema1::CheckCollisionObstacleD(float x, float y) {
    if ( x == -1.82f)
        return false;

    return true;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)
    //  printf("%f x %f y\n", player.x, player.y);

    glm::ivec2 resolution = window->GetResolution();
    if (window->KeyHold(GLFW_KEY_W)) {
        if (player.y < 1.9f) { //  dc nu se opreste????
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
        /*if (CheckCollisionObstacleD(player.x, player.y) == true) {
            printf("da\n");
        }
       else ("nu\n"); */
        if (player.x < 1.9f){ // && CheckCollisionObstacleD(player.x, player.y)) {
            player.x += deltaTime;
            if (!projectile.isMoving)
                projectile.x = player.x;

        }
    }
    // TODO(student): Zoom in and zoom out logic window with Z and X
    if (window->KeyHold(GLFW_KEY_Z)) {
        logicSpace.height += deltaTime;
        logicSpace.width += deltaTime;
        logicSpace.x += deltaTime / 2;  // todo nu merg astea, se scaleaza din colt
        logicSpace.y += deltaTime / 2;  // cu astea se poate face logicspace-ul centrat pe jucator ?? la wasd

    }
    if (window->KeyHold(GLFW_KEY_X)) {
        logicSpace.height -= deltaTime;
        logicSpace.width -= deltaTime;
        logicSpace.x -= deltaTime / 2;
        logicSpace.y -= deltaTime / 2;
    }

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
    float dy = resolution.y - player.y - cursorY;
    float dx = cursorX - player.x;
    player.angle = atan(dy / dx) / M_PI * 180.0f;
    if (!projectile.isMoving)
        projectile.angle = player.angle;
    
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

#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>
#include <math.h>


#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();

    player.lives = lives = 1.0f;
    player.angle = projectile.angle = 0;
    player.x = projectile.x =  0;
    player.y = projectile.y = 0;
    projectile.length = 10;
    projectile.shot = false;
    projectile.sec = 0;
    obstacle.x = obstacle.y = 0;
    score = 0;
    maxScore = 5;  // change this if you want to play longer
    enemy_timer = 0;
    game = 1;
    

    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3((player.x + 2.0f) * resolution.x / 4 - resolution.x / 2, (player.y + 2.0f) * resolution.y / 4 - resolution.y / 2, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    Mesh* healthBar = object2D::CreateSquare1("healthBar", glm::vec3(0, 0, 0), 0.7f, 0.10f, glm::vec3(0.850, 0.792, 0.701), true);
    AddMeshToList(healthBar);

    Mesh* healthBar_wf = object2D::CreateSquare1("healthBar_wf", glm::vec3(0, 0, 0), 0.7f, 0.10f, glm::vec3(0.850, 0.792, 0.710), false);
    AddMeshToList(healthBar_wf);

    numberOfObstacles = 9;
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
        numberOfEnemies = 10; 
        
        for (int i = 0; i < numberOfEnemies; ++i) {
            enemy_aux.width = 0.3f;
            enemy_aux.height = 0.3f;
            enemy_aux.x = ((i + 1) * rand()) % 2; 
            enemy_aux.y = ((i + 1) * rand()) % 2;
            
            enemy_aux.diffX = 4.0f - enemy_aux.x;
            enemy_aux.diffY = 4.0f - enemy_aux.y;

            enemy_aux.onScreen = true;
            enemy_aux.color = glm::vec3(0.596, 0.427, 0.556);
            enemy_aux.scale = 1;
            enemy.push_back(enemy_aux);

        }
    }
    Mesh* enemy_new;
    enemy_new = object2D::CreateEnemy("enemy", glm::vec3(0, 0, 0), enemy_aux.color, glm::vec3(0, 0, 0));
    AddMeshToList(enemy_new);
    
    printf("HELLO! GOOD LUCK PLAYING SURVIVAL SHOOTER - MICKEY MOUSE EDITION!\n\n");
    printf("Maximum score is %d and you start at %d score. You have %f lives, don't lose them!\n", maxScore, score, lives);
}

bool Tema1::checkProjectileEnemyCollision(int i) {
    if (!enemy[i].onScreen)
        return false;
    float a = enemy[i].width;
    float b = enemy[i].height;

    float x = (projectile.x + 0.03f * cos(projectile.angle)) - enemy[i].x + 2.0f;
    float y = (projectile.y + 0.06f * sin(projectile.angle)) - enemy[i].y + 2.0f;

    if (pow(x / a, 2) + pow(y / b, 2) <= 1 && (projectile.x != player.x || projectile.y != player.y)) {
        enemy[i].onScreen = false;
        projectile.x = player.x;
        projectile.y = player.y;
        projectile.shot = false;
        score += 1;
        if (score == maxScore) {
            printf("Congratulations! you are the winner! :)\n");
            Exit();
        } else 
            printf("~~~~~~Your score is %d! Keep playing!~~~~~~~\n", score);
        return true;
    }
    return false;
}

bool Tema1::checkProjectileObstacleCollision(int i) { 
    
    float a = 0.5f;
    float b = 0.5f;

    float x = (projectile.x + 0.03f * cos(projectile.angle)) - obstacle_struct[i].x + 2.0f;
    float y = (projectile.y + 0.06f * sin(projectile.angle)) - obstacle_struct[i].y + 2.0f;

    if (pow(x / a, 2) + pow(y / b, 2) <= 1 && (projectile.x != player.x || projectile.y != player.y)) {
        projectile.x = player.x;
        projectile.y = player.y;
        projectile.shot = false;
        return true;
    }
    return false;
}

bool Tema1::checkPlayerObstacleCollision(int i) {
    float a = 0.5f;
    float b = 0.5f;

    float x = (player.x + 2.0f) - obstacle_struct[i].x;
    float y = (player.y + 2.0f) - obstacle_struct[i].y;

    if (pow(x / a, 2) + pow(y / b, 2) <= 1) {
        return true;
    }
    return false;
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
    return (projectile.x > 0.75 + player.x || projectile.y < -0.75 + player.y ||
        projectile.y > 0.75 + player.y || projectile.x < -0.75 + player.x   // distanta de propagare 
        || projectile.x > 1.9 || projectile.x < -1.9 || projectile.y < -1.9   // sa nu depaseasca harta
        || projectile.y > 1.9);
}

bool Tema1::positionOutOfBonds(float x, float y) {
    return (x > 4 || y < 0  || y > 4 || x < 0);
}

void Tema1::ResetProjectile() {
    projectile.x = player.x;
    projectile.y = player.y;
    projectile.angle = player.angle + 3 * M_PI / 2;
    projectile.shot = false;
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3((player.x + 2.0f) * resolution.x / 4 - resolution.x / 2 ,
            (player.y + 2.0f) * resolution.y / 4 - resolution.y / 2, 50));

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.937, 0.890, 0.815), true);

    // Compute the 2D visualization matrix
    visMatrix_outside = glm::mat3(1);
    visMatrix_outside *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix_outside);

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y); 
    SetViewportArea(viewSpace, glm::vec3(0.937, 0.890, 0.815), true);

    // Compute the 2D visualization matrix
    visMatrix_inside = glm::mat3(1);
    visMatrix_inside *= VisualizationTransf2D(logicSpace, viewSpace);
    
    //projectile movement
    {
        if (projectile.shot) {
            projectile.x += projectile.power * deltaTimeSeconds * 2.f * cos(projectile.angle);
            projectile.y += projectile.power * deltaTimeSeconds * 2.f * sin(projectile.angle);
        }

        for (int i = 0; i < numberOfEnemies; ++i) {
            checkProjectileEnemyCollision(i);
        }

       for (int i = 0; i < 8; i++) {
            checkProjectileObstacleCollision(i);
        }

        if (projectileOutOfBounds())
            ResetProjectile();
    }
    srand(time(NULL));

    // Enemy Movement
    {  
        for (int i = 0; i < numberOfEnemies; ++i) {
            if (enemy[i].onScreen) {
                if (player.y + 2.0f > enemy[i].y) { 
                    do { 
                        enemy[i].y += deltaTimeSeconds * (((i / 10 + 1) * rand()) % 4) / 5;
                    } while (enemy[i].y + enemy[i].diffY - 2.0f == player.y + 2.0f);
                }
                if(player.y + 2.0f < enemy[i].y) {
                    do {
                        enemy[i].y -= deltaTimeSeconds * (((i / 10 + 1) * rand()) % 4) / 5;
                    } while (enemy[i].y + enemy[i].diffY - 2.0f == player.y + 2.0f);                   
                }

                if (player.x + 2.0f > enemy[i].x) {
                    do {
                        enemy[i].x += deltaTimeSeconds * (((i / 10 + 1) * rand()) % 4) / 5;
                    } while (enemy[i].x + enemy[i].diffX - 2.0f == player.y + 2.0f);                  
                }
                if(player.x + 2.0f < enemy[i].x) {
                    do {
                        enemy[i].x -= deltaTimeSeconds * (((i / 10 + 1) * rand()) % 4) / 5;
                    } while (enemy[i].x + enemy[i].diffX - 2.0f == player.x + 2.0f);                    
                }
                
                if (pow(player.x + 2.7f - enemy[i].x, 2) + pow(player.y + 2.7f - enemy[i].x, 2) <= 1 
                    && enemy[i].onScreen == true) {  
                    enemy[i].onScreen = false;
                    player.lives -= 0.1f;
                    if (player.lives <= 0) {
                        for (int j = 0; j < numberOfEnemies; j++) {
                            enemy[j].onScreen = false;
                            game = 0;
                        }
                    } else
                        printf("You have %f lives left!\n", player.lives);
                }
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
    modelMatrix = visMatrix_inside * transform2D::Translate(player.x + 3.2, player.y + 3.8) * transform2D::Scale(1.0f, 1.0f);
    RenderMesh2D(meshes["healthBar_wf"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix_inside * transform2D::Translate(player.x + 3.2, player.y + 3.8) * transform2D::Scale(player.lives, 1.0f); 
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
        float enemy_clock = std::clock() / CLOCKS_PER_SEC;
        for (int i = 0; i < numberOfEnemies; ++i) {
            if (enemy[i].onScreen == true) { 
                modelMatrix = visMatrix_inside * transform2D::Translate(enemy[i].x, enemy[i].y) * transform2D::Scale(0.3f, 0.3f);
                RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
    obstacle_t aux_obstacle;


    aux_obstacle.x = 0.25f;
    aux_obstacle.y = 0.4f;
    aux_obstacle.scaleX = 0.5f;
    aux_obstacle.scaleY = 1.3f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 0.5f;
    aux_obstacle.y = 2.5f;
    aux_obstacle.scaleX = 0.8f;
    aux_obstacle.scaleY = 0.5f;    
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 0.5f;
    aux_obstacle.y = 3.3f;
    aux_obstacle.scaleX = 0.8f;
    aux_obstacle.scaleY = 0.5f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 0.8f;
    aux_obstacle.y = 1.f;
    aux_obstacle.scaleX = 0.6f;
    aux_obstacle.scaleY = 0.6f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 1.3f;
    aux_obstacle.y = 2.4f;
    aux_obstacle.scaleX = 0.6f;
    aux_obstacle.scaleY = 1.5f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 2.5f;
    aux_obstacle.y = 3.f;
    aux_obstacle.scaleX = 0.5f;
    aux_obstacle.scaleY = 0.8f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 2.2f;
    aux_obstacle.y = 1.4f;
    aux_obstacle.scaleX = 0.9f;
    aux_obstacle.scaleY = 0.9f;
    obstacle_struct.push_back(aux_obstacle);

    aux_obstacle.x = 3.3f;
    aux_obstacle.y = 0.7f;
    aux_obstacle.scaleX = 0.5f;
    aux_obstacle.scaleY = 1.4f;
    obstacle_struct.push_back(aux_obstacle);

    obstacle.x = 3.3f;
    obstacle.y = 2.7f;
    obstacle.scaleX = 0.5f;
    obstacle.scaleY = 1.4f;
    obstacle_struct.push_back(aux_obstacle);

    for (int i = 0; i < obstacle_struct.size(); i++) {
        modelMatrix = visMatrix_outside * transform2D::Translate(obstacle_struct[i].x, obstacle_struct[i].y) 
            * transform2D::Scale(obstacle_struct[i].scaleX, obstacle_struct[i].scaleY);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    }     

    if (game == 0) {
        printf("You lost  :(  Please exit the game!\n"); 
        Exit();
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    glm::ivec2 resolution = window->GetResolution();
    for (int i = 0; i < obstacle_struct.size(); i++) {
        if (!checkPlayerObstacleCollision(i) && window->KeyHold(GLFW_KEY_W)) {
            if (player.y < 1.9f) {
                player.y += deltaTime / obstacle_struct.size();
                if (!projectile.shot)
                    projectile.y = player.y;

            }
        }
        if (!checkPlayerObstacleCollision(i) && window->KeyHold(GLFW_KEY_A)) {
            if (player.x > -1.9f) {
                player.x -= deltaTime / obstacle_struct.size();
                if (!projectile.shot)
                    projectile.x = player.x;

            }
        }
        if (!checkPlayerObstacleCollision(i) && window->KeyHold(GLFW_KEY_S)) {
            if (player.y > -1.9f) {
                player.y -= deltaTime / obstacle_struct.size();
                if (!projectile.shot)
                    projectile.y = player.y;

            }
        }
        if (!checkPlayerObstacleCollision(i) && window->KeyHold(GLFW_KEY_D)) {

            if (player.x < 1.9f) {
                player.x += deltaTime / obstacle_struct.size();
                if (!projectile.shot)
                    projectile.x = player.x;

            }
        }
    }
   
    float mytime = std::clock() / CLOCKS_PER_SEC + 1;
    if (!projectile.shot && window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && mytime - projectile.sec > 1) {
        projectile.shot = true;
        projectile.sec = mytime;
        projectile.power = 1.1;
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
    if (!projectile.shot)
        projectile.angle = player.angle + 3 * M_PI / 2;
    
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
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

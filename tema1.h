#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void Tema1::setPlayerAngle();
        bool Tema1::checkProjectileObstacleCollision(int i);
        bool Tema1::checkPlayerObstacleCollision(int i);
        bool Tema1::checkProjectileEnemyCollision(int i);
        bool Tema1::projectileOutOfBounds();
        void Tema1::ResetProjectile();
        bool Tema1::positionOutOfBonds(float x, float y);


        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix_outside, visMatrix_inside;

        int cursorX, cursorY;

        struct player_t {
            int speed;
            float x, y, angle;
            float lives;
        } player;

        struct projectile_t {
            float length, angle, power, x, y;
            bool shot;
            float sec;
        } projectile;

        struct obstacle_t {
            float x, y;
            float scaleX, scaleY;
        } obstacle;
        std::vector <struct obstacle_t> obstacle_struct;

        struct enemy_t {
            float width, height;
            bool onScreen;
            float x, y, scale, angle;
            glm::vec3 color;
            float diffX, diffY;
        } enemy_aux;
        std::vector <struct enemy_t> enemy;

        float l;
        int numberOfEnemies, numberOfEnemies_current;
        int numberOfObstacles;
        int score, maxScore;
        float lives;
        float enemy_timer;
        int game;

    };
}   


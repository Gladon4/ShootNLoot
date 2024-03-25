#include <stdio.h>
#include "string.h"
#include "stdlib.h"

#include "raylib.h"
#include "raymath.h"

#include "entity_manager.h"
#include "smooth_cam.h"
#include "player.h"
#include "level.h"
#include "input.h"
#include "tool.h"

#define VERSION "0.0.0.1 pre-alpha"

int main()
{
    printf("Loot N Shoot! %s\n", VERSION);

    Vector2 screeSize = (Vector2){800, 600};

    char windowTitle[100];
    strcat(windowTitle, "Loot N Shoot ");
    strcat(windowTitle, VERSION);

    InitWindow(screeSize.x, screeSize.y, windowTitle);

    Level level = CreateLevel((Vector2) {2000, 2000}, (Vector2) {250,250}, "resources/testLevel.png", 123);

    EntityManager entityManager = CreateEntityManager(&level, 1000);

    Entity* player = CreatePlayer(&entityManager, (Vector2) {1000, 1000});

    Tool* pistol = CreatePistol("Cool Pistol", 100, 0.2);
    player->currentTool = pistol;

    EntityManagerCreateEntity(&entityManager, "", (Vector2) {1000, 1200}, "test", "test_pushable", 0.1, (Vector2) {175, 175}, 0.1, 100);
    EntityManagerCreateEntity(&entityManager, "resources/tree.png", (Vector2) {1200, 1200}, "test", "immovable", 0.1, (Vector2) {174/2, 316/2}, 0.1, 100);

    SmoothCam camera = CreateSmoothCamera(Vector2Zero(), screeSize, player, 5);
    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        HandleInput(player, &camera, &entityManager, deltaTime);

        UpdateEntities(&entityManager, deltaTime);
        UpdateCameraPosition(&camera, level, deltaTime);

        BeginDrawing();
        BeginMode2D(camera.camera);

        ClearBackground(RAYWHITE);
        DrawLevel(&level);

        DrawEntities(&entityManager);

        DrawCircle(player->position.x + player->size.x/2 + player->currentToolPosition.x, player->position.y + player->size.y/2 + player->currentToolPosition.y, 15, GREEN);        

        DebugDrawChunksBorders(&level);
        EndMode2D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
#include <stdio.h>
#include "string.h"

#include "raylib.h"
#include "raymath.h"

#include "entity_manager.h"
#include "smooth_cam.h"
#include "player.h"
#include "level.h"
#include "input.h"

#define VERSION "0.0.0.1 pre-alpha"

int main()
{
    printf("Loot N Shoot! %s\n", VERSION);


    Vector2 screeSize = (Vector2){800, 600};

    char windowTitle[100];
    strcat(windowTitle, "Loot N Shoot ");
    strcat(windowTitle, VERSION);

    InitWindow(screeSize.x, screeSize.y, windowTitle);

    Level level = CreateLevel((Vector2) {2000, 2000}, (Vector2) {500, 500}, 123);

    EntityManager entityManager = CreateEntityManager(&level, 1000);

    Entity* player = CreatePlayer(&entityManager, (Vector2) {1000, 1000});
    EntityManagerCreateEntity(&entityManager, (Vector2) {1000, 1200}, "test", "test_pushable", 0.1, (Vector2) {200, 200}, 0.1);
    EntityManagerCreateEntity(&entityManager, (Vector2) {1200, 1200}, "test", "pushable_test", 0.1, (Vector2) {50, 150}, 0.1);

    SmoothCam camera = CreateSmoothCamera(Vector2Zero(), screeSize, player, 5);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        HandleInput(player, deltaTime);

        UpdateEntities(&entityManager, deltaTime);
        UpdateCameraPosition(&camera, level, deltaTime);

        BeginDrawing();
        BeginMode2D(camera.camera);

        ClearBackground(RAYWHITE);
        DrawLevel(&level);

        DrawEntities(&entityManager);

        DebugDrawChunksBorders(&level);
        EndMode2D();
        
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
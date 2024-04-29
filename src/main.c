#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "raylib.h"
#include "raymath.h"

#include "entity_manager.h"
#include "smooth_cam.h"
#include "player.h"
#include "level.h"
#include "input.h"
#include "tool.h"

#include "sprite_manager.h"

#define VERSION "0.0.0.2 pre-alpha"

int main()
{
    printf("Loot N Shoot! %s\n", VERSION);

    Vector2 screeSize = (Vector2){800, 600};

    char windowTitle[100];
    strcat(windowTitle, "Loot N Shoot ");
    strcat(windowTitle, VERSION);    

    InitWindow(screeSize.x, screeSize.y, windowTitle);

    Texture2D* sprites = loadSprites();

    int seed = 123940;
    Level level = CreateLevel( sprites[TEST_LEVEL_SPRITE], (Vector2) {2000, 2000}, (Vector2) {200, 200}, seed);

    EntityManager entityManager = CreateEntityManager(&level, 1000);
    entityManager.sprites = sprites;

    LevelGenerator(&level, &entityManager, seed);

    Entity* player = CreatePlayer(&entityManager, (Vector2) {1000, 1000});

    Tool* pistol = CreatePistol("Cool Pistol", 100, 0.15);
    player->currentTool = pistol;

    // EntityManagerCreateEntity(&entityManager, TREE_SPITE, (Vector2) {1200, 1200}, "test", "immovable", 0.1, (Vector2) {100, 160}, (Rectangle) {25, 30, 50, 110}, 0.1, 100);
    // Entity* testEnt = EntityManagerCreateEntity(&entityManager, NULL, (Vector2) {1000, 1200}, TEST_H_TAG, PUSHABLE_E_TAG, 0.1, (Vector2) {175, 175}, (Rectangle) {0,0, 175, 175}, 0.1, 100);

    SmoothCam camera = CreateSmoothCamera(Vector2Zero(), screeSize, player, 5);
    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        struct timeval frame_start, update_end, draw_end;
        gettimeofday(&frame_start, NULL);

        float deltaTime = GetFrameTime();

        // Updates
        HandleInput(player, &camera, &entityManager, deltaTime);

        UpdateEntities(&entityManager, deltaTime);
        UpdateCameraPosition(&camera, level, deltaTime);

        gettimeofday(&update_end, NULL);

        // Drawing
        BeginDrawing();
        BeginMode2D(camera.camera);

        ClearBackground(RAYWHITE);
        DrawLevel(&level);

        DrawEntities(&entityManager);

        DrawCircle(player->position.x + player->size.x/2 + player->currentToolPosition.x, player->position.y + player->size.y/2 + player->currentToolPosition.y, 15, GREEN);        

        // DebugDrawChunksBorders(&level);
        EndMode2D();

        DrawFPS(10, 10);

        EndDrawing();
    
        gettimeofday(&draw_end, NULL);
        long update_time = (update_end.tv_sec - frame_start.tv_sec) * 1000000 + update_end.tv_usec - frame_start.tv_usec;
        long draw_time = (draw_end.tv_sec - update_end.tv_sec) * 1000000 + draw_end.tv_usec - update_end.tv_usec; 
    
        // printf("Update Time: %lu us \t Draw Time: %lu us \n", update_time, draw_time);
    }

    CloseWindow();
    
    return 0;
}
#include "input.h"

#include "raylib.h"
#include "raymath.h"

#define playerSpeed 2.0

void HandleInput(Entity* player, SmoothCam* camera, EntityManager* entityManager, float deltaTime)
{
    if (IsKeyDown(KEY_A)) {player->velocity.x -= playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_D)) {player->velocity.x += playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_W)) {player->velocity.y -= playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_S)) {player->velocity.y += playerSpeed * deltaTime * 60;}
    
    CalculateToolPosition(player, camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        player->currentTool->toolAction(player->currentTool, entityManager, player);
    }

}

void CalculateToolPosition(Entity* player, SmoothCam* camera) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mousePosWorld = GetScreenToWorld2D(mousePos, camera->camera);
    Vector2 lookDir = Vector2Subtract(mousePosWorld, Vector2Add(player->position, Vector2Scale(player->size, 0.5)));
    lookDir = Vector2Normalize(lookDir);
    Vector2 toolPos = Vector2Scale(lookDir, 75);

    player->currentToolPosition = toolPos;
}
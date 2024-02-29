#include "input.h"

#define playerSpeed 2.0

void HandleInput(Entity* player, float deltaTime)
{
    if (IsKeyDown(KEY_A)) {player->velocity.x -= playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_D)) {player->velocity.x += playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_W)) {player->velocity.y -= playerSpeed * deltaTime * 60;}
    if (IsKeyDown(KEY_S)) {player->velocity.y += playerSpeed * deltaTime * 60;}
}
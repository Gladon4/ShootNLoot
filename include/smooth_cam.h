#ifndef SMOOTH_CAM_H
#define SMOOTH_CAM_H

#include "raylib.h"
#include "entity.h"
#include "level.h"

typedef struct SmoothCam
{
    Camera2D camera;
    Vector2 position;
    Vector2 size;
    Entity* target;
    float speed;
} SmoothCam;

SmoothCam CreateSmoothCamera(Vector2 startPosition, Vector2 size, Entity* target, float speed);

void UpdateCameraPosition(SmoothCam* camera, Level level, float deltaTime);

#endif
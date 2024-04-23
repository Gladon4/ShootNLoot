#ifndef HITBOX_H
#define HITBOX_H

#include "raylib.h"

#define TEST_H_TAG 1
#define PLAYER_H_TAG 2

typedef struct Hitbox
{
    Rectangle rect;
    int tags;
} Hitbox;

#endif

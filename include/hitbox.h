#ifndef HITBOX_H
#define HITBOX_H

#include "raylib.h"

typedef struct Hitbox
{
    Rectangle rect;
    char* tag;
} Hitbox;

#endif

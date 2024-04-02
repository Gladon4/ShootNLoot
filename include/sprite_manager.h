#ifndef SPRITE_MANAGER
#define SPRITE_MANAGER

#include "raylib.h"
#include "stdlib.h"

typedef struct Sprites 
{
    Texture2D unknownTexture;
    Texture2D treeTexture;

    Texture2D unknownLevelTexture;
} Sprites;

Sprites* createSpriteStruct();

// #define SpriteManager createSpriteStruct()

#endif
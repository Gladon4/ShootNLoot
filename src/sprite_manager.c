#include "sprite_manager.h"
#include "stdio.h"

Sprites* createSpriteStruct()
{
    Sprites* sprites = malloc(sizeof(Sprites));

    if (sprites == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    *sprites = (Sprites) {
        .unknownTexture = LoadTexture("resources/noSprite.png"),
        .treeTexture = LoadTexture("resources/tree.png"),

        .unknownLevelTexture = LoadTexture("resources/testLevel.png")
    };

    return sprites;
}
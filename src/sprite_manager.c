#include "sprite_manager.h"
#include "stdio.h"

Texture2D* loadSprites()
{
    Texture2D* sprites = calloc(NUMBER_OF_SPRITES, sizeof(Texture2D));

    if (sprites == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    sprites[NO_SPRITE] = LoadTexture("resources/noSprite.png");
    sprites[TREE_SPITE] = LoadTexture("resources/tree.png");
    sprites[TEST_LEVEL_SPRITE] = LoadTexture("resources/testLevel.png");

    return sprites;
}
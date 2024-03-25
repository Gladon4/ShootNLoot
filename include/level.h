#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "hitbox.h"
#include "chunk.h"

typedef struct Level
{
    Vector2 size;

    float brightness;

    Vector2 chunkSize;
    Chunk* chunks;
    Vector2 numberOfChunks;

    Texture2D baseTexture;
} Level;


Level CreateLevel(Vector2 levelSize, Vector2 chunkSize, char* baseTexturePath, int seed);

void DrawLevel(Level* level);

void DebugDrawChunksBorders(Level* level);

#endif

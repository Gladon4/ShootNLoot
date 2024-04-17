#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "hitbox.h"
#include "chunk.h"

#define NUMBER_OF_TREES 100

struct EntityManager;

typedef struct Level
{
    Vector2 size;

    float brightness;

    Vector2 chunkSize;
    Chunk* chunks;
    Vector2 numberOfChunks;

    Texture2D baseTexture;
} Level;


Level CreateLevel(Texture2D levelBaseTexture, Vector2 levelSize, Vector2 chunkSize, int seed);

void DrawLevel(Level* level);

void DebugDrawChunksBorders(Level* level);

void LevelGenerator(Level* level, struct EntityManager* entityManager, int seed);

#endif

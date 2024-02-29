#ifndef CHUNK_H
#define CHUNK_H

#include "raylib.h"

#include "hitbox.h"
#include "entity.h"

typedef struct Chunk
{
    Entity** entitiesInChunk;
    int numberOfEntities;
} Chunk;


#endif
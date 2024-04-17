#include "level.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "stdlib.h"

#include "entity.h"
#include "sprite_manager.h"

Entity* CreateTree(struct EntityManager* entityManager, Vector2 position);

Level CreateLevel(Texture2D levelBaseTexture, Vector2 levelSize, Vector2 chunkSize, int seed)
{
    Vector2 numberOfChunks;
    numberOfChunks.x = levelSize.x / chunkSize.x;
    numberOfChunks.y = levelSize.y / chunkSize.y;
    Chunk* chunks = (Chunk*)calloc(numberOfChunks.x * numberOfChunks.y, sizeof(Chunk));

    if (chunks == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    Level level = (Level)
    {
        .chunkSize = chunkSize,
        .brightness = 0.1,
        .size = levelSize,
        .chunks = chunks,
        .numberOfChunks = numberOfChunks,
        .baseTexture = levelBaseTexture
    };

    return level;
}

void DrawLevel(Level* level)
{
    DrawTexture(level->baseTexture, 0, 0, WHITE);
}

void LevelGenerator(Level* level, struct EntityManager* entityManager, int seed)
{
    srand(seed);

    int numberOfTrees = NUMBER_OF_TREES + (rand() % NUMBER_OF_TREES);
    
    for (int i=0; i<numberOfTrees; i++)
    {
        int xPosition = rand() % (int)level->size.x;
        int yPosition = rand() % (int)level->size.y;
        CreateTree(entityManager, (Vector2){xPosition, yPosition});
    }
}

void DebugDrawChunksBorders(Level* level)
{
    for (int i=0; i<level->numberOfChunks.x; i++)
    {
        DrawLine(i*level->chunkSize.x, 0, i*level->chunkSize.x, level->size.y, GREEN);
    }

    for (int j=0; j<level->numberOfChunks.y; j++)
    {
        DrawLine(0, j*level->chunkSize.y, level->size.x, j*level->chunkSize.y, GREEN);
    }

    for (int j=0; j<level->numberOfChunks.y; j++)
    {
        for (int i=0; i<level->numberOfChunks.x; i++)
        {
            const int chunkIndex = j*level->numberOfChunks.x + i;
            const int entitiesInChunk = level->chunks[chunkIndex].numberOfEntities;

            char numberText[20];
            sprintf(numberText, "Entities: %d", entitiesInChunk);

            DrawText(numberText, i*level->chunkSize.x+30, j*level->chunkSize.y+60, 30, RED);

            char numberID[10];
            sprintf(numberText, "ID: %d", chunkIndex);

            DrawText(numberText, i*level->chunkSize.x+30, j*level->chunkSize.y+30, 30, RED);
        }
    }
}
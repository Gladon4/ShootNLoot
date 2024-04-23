#include "entity_manager.h"
#include "stdlib.h"
#include "string.h"
#include "chunk.h"
#include "collision_system.h"
#include "stdio.h"
#include "raymath.h"

void _CollisionPrevention(Level* level, int chunkIndex);
void _UpdateEntityChunks(EntityManager* entityManager);
void _SortEntities(EntityManager* entityManager);
void _QuickSortEntities(Entity** entities, int left, int right);

EntityManager CreateEntityManager(Level* level, const int maxNumberOfEntities)
{
    Entity** entities = calloc(maxNumberOfEntities, sizeof(Entity*));
    if (entities == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    return (EntityManager) {
        .entities = entities,
        .numberOfEntities = 0,
        .currentUUID = 0,
        .level = level
    };
}

Entity* EntityManagerCreateEntity(EntityManager* entityManager,  int spriteID, Vector2 position, int hitboxTags, int entityTags, float mass, Vector2 spriteSize, Rectangle hitboxRectangle, float drag, int hp)
{
    if (spriteID < 0 || !spriteID) spriteID = NO_SPRITE;

    Entity* entity = CreateEntity(entityManager->sprites[spriteID], position, hitboxTags, entityTags, mass, spriteSize, hitboxRectangle, drag, hp, entityManager->currentUUID);
    
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
    entityManager->currentUUID++;

    return entity;
}

Entity* CreateBullet(EntityManager* entityManager, Vector2 position, Vector2 velocity, int entityTags, Vector2 size, float drag, int hp) {
    Rectangle rectangle = (Rectangle) {
        .x = 0,
        .y = 0,
        .width = size.x,
        .height = size.y
    };

    Entity* bullet = EntityManagerCreateEntity(entityManager, NULL, position, 0, entityTags, 1, size, rectangle, drag, hp);
    bullet->velocity = velocity;

    return bullet;
}

Entity* CreateTree(EntityManager* entityManager, Vector2 position)
{
    return EntityManagerCreateEntity(entityManager, TREE_SPITE, position, TEST_H_TAG, IMMOVABLE_E_TAG+TREE_E_TAG, 0.1, (Vector2) {100, 160}, (Rectangle) {30, 90, 30, 50}, 0, 100);
}

void AddEntityToEntityManager(EntityManager* entityManager, Entity* entity) 
{
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
}

void RemoveEntityFromEntityManager(EntityManager* entityManager, Entity* entity)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        if (entityManager->entities[i]->uuid == entity->uuid)
        {
            entityManager->entities[i] = entityManager->entities[entityManager->numberOfEntities-1];
            entityManager->numberOfEntities--;
            return;
        }
    }
}

void UpdateEntities(EntityManager* entityManager, float deltaTime)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        UpdateEntity(entityManager->entities[i], entityManager->level->size, deltaTime);
        entityManager->entities[i]->state = idle;
    }

    _UpdateEntityChunks(entityManager);
    _SortEntities(entityManager);

    for (int j=0; j<5; j++)
    {
        const int totalNumberOfChunks = entityManager->level->numberOfChunks.x * entityManager->level->numberOfChunks.y;
        for (int i=0; i<totalNumberOfChunks; i++)
        {
            _CollisionPrevention(entityManager->level, i);
        }    
    }

    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        if (entityManager->entities[i]->currentHP <= 0)
        {
            Entity* entityToRemove = entityManager->entities[i];
            RemoveEntityFromEntityManager(entityManager, entityToRemove);
            free(entityToRemove);
        }
    }
}

void _CollisionPrevention (Level* level, int chunkIndex)
{
    const int chunkIndexOffests[] = {-level->numberOfChunks.x-1, -level->numberOfChunks.x, -level->numberOfChunks.x+1,
                                                        -1,                        0,                         +1,
                                +level->numberOfChunks.x-1, +level->numberOfChunks.x, +level->numberOfChunks.x+1,};

    Chunk chunk = level->chunks[chunkIndex];

    for (int i=0; i<chunk.numberOfEntities; i++)
    {
        for (int o=0; o<9; o++)
        {
            const bool offestedChunkDoesNotExists = (chunkIndexOffests[o] < 0 || (chunkIndex + chunkIndexOffests[o]) >= level->numberOfChunks.x * level->numberOfChunks.y);
            if (offestedChunkDoesNotExists) {continue;}

            Chunk chunk2 = level->chunks[chunkIndex + chunkIndexOffests[o]];

            for (int j=0; j<chunk2.numberOfEntities; j++)
            {
                if (chunk.entitiesInChunk[i]->uuid == chunk2.entitiesInChunk[j]->uuid) {continue;}
                 
                CollisionPrevention(chunk.entitiesInChunk[i], chunk2.entitiesInChunk[j]);
                // UpdateEntityHitBox(chunk2.entitiesInChunk[j]);
            }
        }
        // UpdateEntityHitBox(chunk.entitiesInChunk[i]);
    }

    
}


void _UpdateEntityChunks (EntityManager* entityManager)
{
    for (int j=0; j<entityManager->level->numberOfChunks.y; j++)
    {
        for (int i=0; i<entityManager->level->numberOfChunks.x; i++)
        {
            int index = entityManager->level->numberOfChunks.x * j + i;

            free(entityManager->level->chunks[index].entitiesInChunk); 
            Entity** entities = calloc(1000, sizeof(Entity*)); 
            if (entities == NULL) 
            {
                printf("memory allocation failed");
                exit(1);
            }

            entityManager->level->chunks[index].entitiesInChunk = entities;
            entityManager->level->chunks[index].numberOfEntities = 0;
        }
    }
    
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        Vector2 chunkCoordinates;

        chunkCoordinates.x = (int)(entityManager->entities[i]->position.x / entityManager->level->chunkSize.x);
        chunkCoordinates.y = (int)(entityManager->entities[i]->position.y / entityManager->level->chunkSize.y);

        const int chunkIndex = chunkCoordinates.y * entityManager->level->numberOfChunks.x + chunkCoordinates.x;
        const int highestEnityIndexInChunk = entityManager->level->chunks[chunkIndex].numberOfEntities;

        entityManager->level->chunks[chunkIndex].entitiesInChunk[highestEnityIndexInChunk] = entityManager->entities[i];
        entityManager->level->chunks[chunkIndex].numberOfEntities++;
    }
}


void _QuickSortEntities(Entity** entities, int left, int right)
{
    Entity* tempPointer;
    Entity* entityX;
    int i,j;

    
    if (left < right)
    {
        entityX = entities[left];
        i = left+1;
        j = right;

        while (i <= j)
        {
            while (i <= j && entities[i]->position.y + entities[i]->size.y < entityX->position.y + entityX->size.y)
            {
                i++;
            }
            while (i <= j && entities[j]->position.y + entities[i]->size.y >= entityX->position.y + entityX->size.y)
            {
                j--;
            }
            if (i < j)
            {
                tempPointer = entities[i];
                entities[i] = entities[j];
                entities[j] = tempPointer;
            }

        }

        i--;
        entities[left] = entities[i];
        entities[i] = entityX;

        _QuickSortEntities(entities, left, i-1);
        _QuickSortEntities(entities, i+1, right);
    }
}


void _SortEntities(EntityManager* entityManager)
{
    _QuickSortEntities(entityManager->entities, 0, entityManager->numberOfEntities-1);
}


void DrawEntities(EntityManager* entityManager)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        // Draw Distance, but it seems raylib is already doing something on its own.
        //
        // if (Vector2Distance(player->position, entityManager->entities[i]->position) >= 200)
        // {
        //     continue;
        // }

        DrawEntity(entityManager->entities[i]);
    }
}
#include "entity_manager.h"
#include "stdlib.h"
#include "string.h"
#include "chunk.h"
#include "collision_system.h"


void _CollisionPrevention (Level* level, int chunkIndex);
void _UpdateEntityChunks (EntityManager* entityManager);

EntityManager CreateEntityManager(Level* level, const int maxNumberOfEntities)
{
    return (EntityManager) {
        .entities = calloc(maxNumberOfEntities, sizeof(Entity*)),
        .numberOfEntities = 0,
        .currentUUID = 0,
        .level = level
    };
}

Entity* EntityManagerCreateEntity(EntityManager* entityManager, Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag)
{
    char* hitboxTagPointer = (char*)malloc(100);
    char* entityTagsPointer = (char*)malloc(100);

    strncpy(hitboxTagPointer, hitboxTag, 100);
    strncpy(entityTagsPointer, entityTags, 100);

    Entity* entity = CreateEntity(position, hitboxTagPointer, entityTagsPointer, mass, size, drag, entityManager->currentUUID);
    
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
    entityManager->currentUUID++;

    return entity;
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
    
    for (int j=0; j<5; j++)
    {
        const int totalNumberOfChunks = entityManager->level->numberOfChunks.x * entityManager->level->numberOfChunks.y;
        for (int i=0; i<totalNumberOfChunks; i++)
        {
            _CollisionPrevention(entityManager->level, i);
        }    
    }
}

void _CollisionPrevention (Level* level, int chunkIndex)
{
    const int indexOffests[] = {-level->numberOfChunks.x-1, -level->numberOfChunks.x, -level->numberOfChunks.x+1,
                                                        -1,                        0,                         +1,
                                +level->numberOfChunks.x-1, +level->numberOfChunks.x, +level->numberOfChunks.x+1,};

    Chunk chunk = level->chunks[chunkIndex];

    for (int i=0; i<chunk.numberOfEntities; i++)
    {
        for (int o=0; o<9; o++)
        {
            const bool offestedChunkDoesNotExists = (indexOffests[o] < 0 || (chunkIndex + indexOffests[o]) >= level->numberOfChunks.x * level->numberOfChunks.y);
            if (offestedChunkDoesNotExists) {continue;}

            Chunk chunk2 = level->chunks[chunkIndex + indexOffests[o]];

            for (int j=0; j<chunk2.numberOfEntities; j++)
            {
                if (chunk.entitiesInChunk[i]->uuid == chunk2.entitiesInChunk[j]->uuid) {continue;}
                 
                CollisionPrevention(chunk.entitiesInChunk[i], chunk2.entitiesInChunk[j]);
                UpdateEntityHitBox(chunk2.entitiesInChunk[j]);
            }
        }
        UpdateEntityHitBox(chunk.entitiesInChunk[i]);
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
            entityManager->level->chunks[index].entitiesInChunk = calloc(100, sizeof(Entity*));

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


void DrawEntities(EntityManager* entityManager)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        DrawEntity(*entityManager->entities[i]);
    }
}
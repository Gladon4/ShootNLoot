#include "entity_manager.h"
#include "stdlib.h"
#include "string.h"
#include "chunk.h"
#include "collision_system.h"
#include "stdio.h"

void _CollisionPrevention (Level* level, int chunkIndex);
void _UpdateEntityChunks (EntityManager* entityManager);

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

Entity* EntityManagerCreateEntity(EntityManager* entityManager,  int spriteID, Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag, int hp)
{
    char* hitboxTagPointer = (char*)malloc(ENTITY_TAG_LENGTH);
    if (hitboxTagPointer == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }
    char* entityTagsPointer = (char*)malloc(ENTITY_TAG_LENGTH);
    if (entityTagsPointer == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    strncpy(hitboxTagPointer, hitboxTag, ENTITY_TAG_LENGTH);
    strncpy(entityTagsPointer, entityTags, ENTITY_TAG_LENGTH);


    if (spriteID < 0 || spriteID == NULL) spriteID = NO_SPRITE;

    Entity* entity = CreateEntity(entityManager->sprites[spriteID], position, hitboxTagPointer, entityTagsPointer, mass, size, drag, hp, entityManager->currentUUID);
    
    entityManager->entities[entityManager->numberOfEntities] = entity;
    entityManager->numberOfEntities++;
    entityManager->currentUUID++;

    return entity;
}

Entity* CreateBullet(EntityManager* entityManager, Vector2 position, Vector2 velocity, char* entityTags, Vector2 size, float drag, int hp) {
    char* entityTagsPointer = (char*)malloc(ENTITY_TAG_LENGTH);
    if (entityTagsPointer == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }
    strncpy(entityTagsPointer, entityTags, ENTITY_TAG_LENGTH);

    
    Entity* bullet = CreateEntity(entityManager->sprites[NO_SPRITE], position, "", entityTagsPointer, 1, size, drag, hp, entityManager->currentUUID);
    bullet->velocity = velocity;

    entityManager->entities[entityManager->numberOfEntities] = bullet;
    entityManager->numberOfEntities++;
    entityManager->currentUUID++;

    return bullet;
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


void DrawEntities(EntityManager* entityManager)
{
    for (int i=0; i<entityManager->numberOfEntities; i++)
    {
        DrawEntity(entityManager->entities[i]);
    }
}
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity.h"
#include "level.h"

typedef struct EntityManager
{
    Entity** entities;
    int numberOfEntities;
    int currentUUID;

    Level* level;
} EntityManager;

EntityManager CreateEntityManager(Level* level, const int maxNumberOfEntities);

Entity* EntityManagerCreateEntity(EntityManager* entityManager, Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag);

void AddEntityToEntityManager(EntityManager* entityManager, Entity* entity);
void RemoveEntityFromEntityManager(EntityManager* entityManager, Entity* entity);

void UpdateEntities(EntityManager* entityManager, float deltaTime);
void DrawEntities(EntityManager* entityManager);

#endif
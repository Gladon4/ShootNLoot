#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "hitbox.h"


typedef enum STATE
{
    idle,
    pushing
} STATE;

typedef struct Entity
{
    float mass;
    Vector2 size;
    float drag;

    Hitbox hitbox;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    int uuid;
    char* tags;
    STATE state;
} Entity;

Entity* CreateEntity(Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag, int uuid);

void UpdateEntity(Entity* entity, Vector2 levelSize, float deltaTime);
void UpdateEntityHitBox(Entity* entity);

void DrawEntity(Entity entity);

bool EntityHasTag(Entity* entity, char* tag);

#endif
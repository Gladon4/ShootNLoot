#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "hitbox.h"
#include "tool.h"

#define ENTITY_TAG_LENGTH 100

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

    int maxHP;
    int currentHP;

    Hitbox hitbox;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    int uuid;
    char* tags;
    STATE state;

    Tool* currentTool;
    Vector2 currentToolPosition;
    float currentToolCooldown;

    Texture2D sprite;
    // void (*damageEntity)(struct Entity* self, struct EntityManager* entityManager);
} Entity;

Entity* CreateEntity(char* spritePath, Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag, int hp, int uuid);

void UpdateEntity(Entity* entity, Vector2 levelSize, float deltaTime);
void UpdateEntityHitBox(Entity* entity);

void DrawEntity(Entity* entity);

bool EntityHasTag(Entity* entity, char* tag);

#endif
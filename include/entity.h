#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "hitbox.h"
#include "tool.h"

// #define ENTITY_TAG_LENGTH 100

#define IMMOVABLE_E_TAG 1
#define PUSHABLE_E_TAG 2
#define TREE_E_TAG 4
#define BULLET_E_TAG 8
#define DAMAGING_E_TAG 16
#define PLAYER_E_TAG 32

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
    // char* tags;
    int tags;
    STATE state;

    Tool* currentTool;
    Vector2 currentToolPosition;
    float currentToolCooldown;

    Texture2D sprite;
    // void (*damageEntity)(struct Entity* self, struct EntityManager* entityManager);
} Entity;

Entity* CreateEntity(Texture2D sprite, Vector2 position, int hitboxTags, int entityTags, float mass, Vector2 spriteSize, Rectangle hitbox, float drag, int hp, int uuid);

void UpdateEntity(Entity* entity, Vector2 levelSize, float deltaTime);
void UpdateEntityHitBox(Entity* entity);

void DrawEntity(Entity* entity);

bool EntityHasTag(Entity* entity, int tag);

#endif
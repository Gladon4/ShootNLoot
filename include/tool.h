#ifndef TOOL_H
#define TOOL_H

#include "raylib.h"
#include "raymath.h"

// #include "entity_manager.h"
// #include "entity.h"

struct EntityManager;
struct Entity;


typedef enum TOOLTYPE
{
    axe,
    pickaxe,
    pistol
} TOOLTYPE;

typedef struct Tool
{
    char* name;

    int maxDurability;
    int durability;

    float cooldownTimeSeconds;

    TOOLTYPE type;

    void (*toolAction)(struct Tool* self, struct EntityManager* entityManager, struct Entity* user);
} Tool;


Tool* CreatePistol(char* name, int ammo, float cooldownTimeSeconds);
void ShootPistol(Tool* self, struct EntityManager* entityManager, struct Entity* user);

#endif 
#include "tool.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "entity_manager.h"

Tool* CreatePistol(char* name, int ammo, float cooldownTimeSeconds) {
    Tool* pistol = malloc(sizeof(Tool));
    char* _name = malloc(sizeof(name));

    strcpy(_name, name);

    pistol->name = _name;

    pistol->maxDurability = ammo;
    pistol->durability = ammo;

    pistol->cooldownTimeSeconds = cooldownTimeSeconds;

    pistol->toolAction = ShootPistol;

    return pistol;
}


void ShootPistol(Tool* self, EntityManager* entityManager, Entity* user) {
    if (self->durability <= 0 || user->currentToolCooldown > 0)
    {
        return;
    }

    printf("Name: %s \n Dur: %d/%d \n CD: %f \n\n", self->name, self->durability, self->maxDurability, self->cooldownTimeSeconds);

    Vector2 velocity = Vector2Scale(user->currentToolPosition, 0.1);
    Vector2 spawnPosition = Vector2Add(user->currentToolPosition, user->position);

    CreateBullet(entityManager, spawnPosition, velocity, "bullet_pistolBullet", (Vector2){10,10}, 0, 10);

    user->currentToolCooldown += self->cooldownTimeSeconds;

    self->durability--;
}

#include "entity.h"

#include "stdlib.h"
#include "string.h"
#include "raymath.h"
#include <stdio.h>

Entity* CreateEntity(char* spritePath, Vector2 position, char* hitboxTag, char* entityTags, float mass, Vector2 size, float drag, int hp, int uuid) {
    Entity* entity = malloc(sizeof(Entity));
    if (entity == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }

    if (!strcmp(spritePath, ""))
    {
        spritePath = "resources/noSprite.png";
    }

    *entity = (Entity)
    {
        .position = position,
        .mass = mass,
        .size = size, 
        .drag = drag,

        .currentHP = hp,
        .maxHP = hp,

        .velocity = (Vector2){0, 0},
        .acceleration = (Vector2) {0, 0},

        .hitbox = (Hitbox)
        {
            .rect = (Rectangle)
            {
                .x = position.x,
                .y = position.y,
                .width = size.x,
                .height = size.y
            },
            .tag = hitboxTag
        },

        .uuid = uuid,
        .tags = entityTags,
        .state = idle,

        .sprite = LoadTexture(spritePath)
    };
    
    return entity;
}

void UpdateEntity(Entity* entity, Vector2 levelSize, float deltaTime)
{
    entity->velocity = Vector2Add(entity->velocity, Vector2Scale(entity->acceleration, deltaTime * 60));
    
    float dragFactor = entity->drag * deltaTime * 60;
    entity->velocity = Vector2Subtract(entity->velocity, Vector2Scale(entity->velocity, dragFactor));

    entity->position = Vector2Add(entity->position, Vector2Scale(entity->velocity, deltaTime * 60));

    entity->position = Vector2Clamp(entity->position, Vector2Zero(), Vector2Subtract(levelSize, entity->size));

    if (entity->currentToolCooldown > 0)
    {
        entity->currentToolCooldown -= deltaTime;
    }

    UpdateEntityHitBox(entity);
}

void UpdateEntityHitBox(Entity* entity)
{
    entity->hitbox.rect = (Rectangle) {
        .x = entity->position.x,
        .y = entity->position.y,
        .width = entity->size.x,
        .height = entity->size.y
    };
}

void DrawEntity(Entity* entity)
{
    // Color col = RED;
    // if(entity.state == pushing) {col = GREEN;}

    // DrawRectangle(entity.position.x - (entity.size.x / 2), entity.position.y - (entity.size.y / 2), entity.size.x, entity.size.y, col);

    Rectangle spriteAtlasRectangle = (Rectangle)
    {
        .x = 0,
        .y = 0, 
        .width = entity->sprite.width,
        .height = entity->sprite.height
    };

    Rectangle worldPositionRecatnge = (Rectangle)
    {
        .x = entity->position.x,
        .y = entity->position.y, 
        .width = entity->size.x, 
        .height = entity->size.y
    };

    DrawTexturePro(entity->sprite, spriteAtlasRectangle, worldPositionRecatnge, Vector2Zero(), 0, WHITE);
}

bool EntityHasTag(Entity* entity, char* tag)
{
    char* entityTags = malloc(ENTITY_TAG_LENGTH);
    if (entityTags == NULL) 
    {
        printf("memory allocation failed");
        exit(1);
    }
    strncpy(entityTags, entity->tags, ENTITY_TAG_LENGTH);

    char* token = strtok(entity->tags, "_");

    while (token != NULL)
    {
        if (!strcmp(token, tag))
        {
            strncpy(entity->tags, entityTags, ENTITY_TAG_LENGTH);
            return true;
        }
        token = strtok(NULL, "_");
    }

    strncpy(entity->tags, entityTags, ENTITY_TAG_LENGTH);
    return false;
}


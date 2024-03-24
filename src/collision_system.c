#include "collision_system.h"
#include "raymath.h"
#include "string.h"
#include "misc_functions.h"
#include "stdio.h"

bool _PushEntity(Entity* pusher, Entity* pushee);
void _DamageEntity(Entity* source, Entity* destination);

void CollisionPrevention(Entity* entity1, Entity* entity2)
{
    if (!CheckCollisionRecs(entity1->hitbox.rect, entity2->hitbox.rect)) 
    {
        return;
    }

    if (EntityHasTag(entity1, "bullet")) 
    {
        _DamageEntity(entity1, entity2);
        return;
    }

    if (EntityHasTag(entity2, "bullet")) 
    {
        _DamageEntity(entity2, entity1);
        return;
    }


    if (EntityHasTag(entity2, "pushable") && !EntityHasTag(entity1, "immovable"))
    {
        if (_PushEntity(entity1, entity2))
        {
            entity1->state = pushing;
            return;
        }
    }

    if (EntityHasTag(entity1, "pushable") && !EntityHasTag(entity2, "immovable"))
    {
        if (_PushEntity(entity2, entity1))
        {
            entity2->state = pushing;
            return;
        }
    }

    
    Rectangle collisionRectangle = GetCollisionRec(entity1->hitbox.rect, entity2->hitbox.rect);

    if (collisionRectangle.width > collisionRectangle.height)
    {
        bool entity1Above = entity2->hitbox.rect.y == collisionRectangle.y;

        if (entity1Above) 
        {
            entity1->acceleration.y = min(entity1->acceleration.y, 0);
            entity2->acceleration.y = max(entity2->acceleration.y, 0);
            
            entity1->velocity.y = min(entity1->velocity.y, 0);
            entity2->velocity.y = max(entity2->velocity.y, 0);

            if (EntityHasTag(entity2, "immovable"))
            {
                entity1->position.y -= collisionRectangle.height;
                return;
            }
            if (EntityHasTag(entity1, "immovable"))
            {
                entity2->position.y += collisionRectangle.height;
                return;
            }

            entity1->position.y -= collisionRectangle.height/2;
            entity2->position.y += collisionRectangle.height/2;
        } 
        else
        {
            entity1->acceleration.y = max(entity1->acceleration.y, 0);
            entity2->acceleration.y = min(entity2->acceleration.y, 0);

            entity1->velocity.y = max(entity1->velocity.y, 0);
            entity2->velocity.y = min(entity2->velocity.y, 0);

            if (EntityHasTag(entity2, "immovable"))
            {
                entity1->position.y += collisionRectangle.height;
                return;
            }            
            if (EntityHasTag(entity1, "immovable"))
            {
                entity2->position.y -= collisionRectangle.height;
                return;
            }

            entity1->position.y += collisionRectangle.height/2;
            entity2->position.y -= collisionRectangle.height/2;

        }
    }
    else
    {
        bool entity1Left = entity2->hitbox.rect.x == collisionRectangle.x;

        if (entity1Left) 
        {
            entity1->acceleration.x = min(entity1->acceleration.x, 0);
            entity2->acceleration.x = max(entity2->acceleration.x, 0);

            entity1->velocity.x = min(entity1->velocity.x, 0);
            entity2->velocity.x = max(entity2->velocity.x, 0);
            
            if (EntityHasTag(entity2, "immovable"))
            {
                entity1->position.x -= collisionRectangle.width;
                return;
            }
            if (EntityHasTag(entity1, "immovable"))
            {
                entity2->position.x += collisionRectangle.width;
                return;
            }

            entity1->position.x -= collisionRectangle.width/2;
            entity2->position.x += collisionRectangle.width/2;
        } 
        else
        {
            entity1->acceleration.x = max(entity1->acceleration.x, 0);
            entity2->acceleration.x = min(entity2->acceleration.x, 0);
            
            entity1->velocity.x = max(entity1->velocity.x, 0);
            entity2->velocity.x = min(entity2->velocity.x, 0);

            if (EntityHasTag(entity2, "immovable"))
            {
                entity1->position.x += collisionRectangle.width;
                return;
            }
            if (EntityHasTag(entity1, "immovable"))
            {
                entity2->position.x -= collisionRectangle.width;
                return;
            }
            
            entity1->position.x += collisionRectangle.width/2;
            entity2->position.x -= collisionRectangle.width/2;
        }
    }
    

    return;
}

bool _PushEntity(Entity* pusher, Entity* pushee)
{
    Rectangle collisionRectangle = GetCollisionRec(pusher->hitbox.rect, pushee->hitbox.rect);
    float weightFactor = pusher->mass / pushee->mass;
    weightFactor = Clamp(weightFactor, 0, 1);

    if (collisionRectangle.width > collisionRectangle.height)
    {
        pusher->velocity.y = pusher->velocity.y * weightFactor;        
        
        bool fromAbove = pushee->hitbox.rect.y == collisionRectangle.y;

        if (fromAbove) 
        {
            pushee->position.y += collisionRectangle.height * 0.5;
            pusher->position.y -= collisionRectangle.height * 0.5;
        } 
        else
        {
            pushee->position.y -= collisionRectangle.height * 0.5;
            pusher->position.y += collisionRectangle.height * 0.5;
        }

        return true;
    }
    else
    {
        pusher->velocity.x = pusher->velocity.x * weightFactor;        
        
        bool fromLeft = pushee->hitbox.rect.x == collisionRectangle.x;

        if (fromLeft) 
        {
            pushee->position.x += collisionRectangle.width * 0.5;
            pusher->position.x -= collisionRectangle.width * 0.5;
        } 
        else
        {
            pushee->position.x -= collisionRectangle.width * 0.5;
            pusher->position.x += collisionRectangle.width * 0.5;
        }

        return true;
    } 
}

void _DamageEntity(Entity* source, Entity* destination) {
    int beforeHitHP = destination->currentHP;
    int afterHitHP = destination->currentHP - source->currentHP;

    destination->currentHP = afterHitHP;
    
    if (afterHitHP <= 0) 
    {
        source->currentHP -= beforeHitHP;
    }
    else
    {
        source->currentHP = 0;
    }

    return;
}
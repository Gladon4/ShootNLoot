#ifndef PLAYER_H
#define PLAYER_H

#include "entity_manager.h"
#include "sprite_manager.h"

Entity* CreatePlayer(EntityManager* entityManager, Vector2 position);

#endif
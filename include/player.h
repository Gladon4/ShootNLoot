#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "entity_manager.h"

Entity* CreatePlayer(EntityManager* entityManager, Vector2 position);

#endif
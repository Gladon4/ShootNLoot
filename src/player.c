#include "player.h"

Entity* CreatePlayer(EntityManager* entityManager, Vector2 position)
{
    return EntityManagerCreateEntity(entityManager, NO_SPRITE, position, "player", "player", 1, (Vector2) {50,50}, (Rectangle) {0, 0, 50, 50}, 0.2, 100);
}

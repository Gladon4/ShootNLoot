#ifndef INPUT_H
#define INPUT_H

#include "smooth_cam.h"
#include "entity_manager.h"

void HandleInput(Entity* player, SmoothCam* camera, EntityManager* entityManager, float deltaTime);
void CalculateToolPosition(Entity* player, SmoothCam* camera);

#endif
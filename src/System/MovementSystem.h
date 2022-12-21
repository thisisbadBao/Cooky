#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"

class MovementSystem: public System {
    public:
        MovementSystem()
        {
            // Todo: 
            // RequrieComponent<TransformComponent>();
        }

        void Update() {
            for (auto entity : GetEntities()) {
                // Todo:Update entity position based on velocity
            }
        }
}

#endif
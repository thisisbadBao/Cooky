#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem: public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update() {
        auto entities = GetSystemEntities();

        // check if entity a collides with b
        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();
            for (auto j = i + 1; j != entities.end(); j++) {
                Entity b = *j;
                if (a == b) continue;
                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();
                
                // AABB collision
                bool collisionHappened = CheckCollision(
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );
                if (collisionHappened) {
                    Logger::Log("Entity " + std::to_string(a.GetId()) + " collides with Entity " + std::to_string(b.GetId()));
                }
            }
        }
    }

    bool CheckCollision (double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
        return (
            aX < bW + bX &&
            aX + aW > bX &&
            aY < bH + bY &&
            aY + aH > bY
        );
    }
};

#endif
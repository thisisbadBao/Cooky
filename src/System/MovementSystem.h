#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

class MovementSystem: public System {
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void Update(double deltaTime) {
        for (auto entity : GetSystemEntities()) {
            // Update entity position based on velocity
            TransformComponent& transform = entity.GetComponent<TransformComponent>();
            const RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            Logger::Log(
                "Entity id = " +
                std::to_string(entity.GetId()) +
                " position is now (" +
                std::to_string(transform.position.x) +
                ", " +
                std::to_string(transform.position.y) + ")"
            );
        }
    }
};

#endif
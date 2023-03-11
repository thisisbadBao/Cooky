#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class MovementSystem: public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvent(const std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Entity a = event.a;
        Entity b = event.b;
        if (a.BelongsToGroup("enemy") && b.BelongsToGroup("obstacle")) {
            Logger::Log("OnEnemyHitsObstacle  a");
            OnEnemyHitsObstacle(a, b);
        }

        if (a.BelongsToGroup("obstacle") && b.BelongsToGroup("enemy")) {
            Logger::Log("OnEnemyHitsObstacle  b");
            OnEnemyHitsObstacle(b, a);
        }
    }

    void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
        Logger::Log("The MovementSystem received an event between " + std::to_string(enemy.GetId()) + " and " + std::to_string(obstacle.GetId()));
        if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
            auto &rigidbody = enemy.GetComponent<RigidBodyComponent>();
            auto &sprite = enemy.GetComponent<SpriteComponent>();

            if (rigidbody.velocity.x != 0) {
                rigidbody.velocity.x *= -1;
                sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            }
            if (rigidbody.velocity.y != 0) {
                rigidbody.velocity.y *= -1;
            }
        }
    }

    void Update(double deltaTime) {
        for (auto entity : GetSystemEntities()) {
            // Update entity position based on velocity
            TransformComponent& transform = entity.GetComponent<TransformComponent>();
            const RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            bool isEntityOutsideMap = (
                transform.position.x < 0 ||
                transform.position.x > Game::mapWidth ||
                transform.position.y < 0 ||
                transform.position.y > Game::mapHeight
            );
            if (isEntityOutsideMap && !entity.HasTag("player")) {
                entity.Kill();
            }
        }
    }
};

#endif
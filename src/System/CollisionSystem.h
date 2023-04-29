#pragma once

#include "../ECS/ECS.h"

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem: public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<EventBus>& eventBus) {
        auto entities = GetSystemEntities();

        // check if entity a collides with b
        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto& aCollider = a.GetComponent<BoxColliderComponent>();
            for (auto j = i + 1; j != entities.end(); j++) {
                Entity b = *j;
                if (a == b) continue;
                auto bTransform = b.GetComponent<TransformComponent>();
                auto& bCollider = b.GetComponent<BoxColliderComponent>();
                
                // AABB collision
                bool collisionHappened = CheckCollision(
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width * aTransform.scale.x,
                    aCollider.height * aTransform.scale.y,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width * bTransform.scale.x,
                    bCollider.height * bTransform.scale.y
                );
                if (collisionHappened) {
                    aCollider.isColliding = true;
                    bCollider.isColliding = true;
                    eventBus->EmitEvent<CollisionEvent>(a, b);
                    Logger::Log("Entity " + std::to_string(a.GetId()) + " collides with Entity " + std::to_string(b.GetId()));
                } else {
                    aCollider.isColliding = false;
                    bCollider.isColliding = false;
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

    void Init(const std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &CollisionSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        int idA = event.a.GetId();
        int idB = event.b.GetId();
        for (auto callbackList : collisionCallbacks) {
            if (callbackList.first == idA) {
                for (auto& callback : callbackList.second) {
                    callback(event.b.GetTag());
                }
            }
            if (callbackList.first == idB) {
                for (auto& callback : callbackList.second) {
                    callback(event.a.GetTag());
                }
            }
        }
    }

    void SetCallbackOnCollision(int entityId, std::function<void(std::string)> callback) {
        if (collisionCallbacks.find(entityId) != collisionCallbacks.end()) {
            collisionCallbacks[entityId].emplace_back(callback);
        } else {
            std::vector<std::function<void(std::string)>> callbacks;
            callbacks.emplace_back(callback);
            collisionCallbacks.emplace(entityId, callbacks);
        }
    }

    void Reset() {
        collisionCallbacks.clear();
    }

private:
    std::map<int, std::vector<std::function<void(std::string)>>> collisionCallbacks;
};
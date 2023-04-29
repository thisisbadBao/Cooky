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

    void Update(std::unique_ptr<EventBus>& eventBus) {}

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
        if (event.beginContact) {
            Logger::LogD("contact point:" + event.points[0].ToString());
            for (auto callbackList : beginContactCallbacks) {
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
        } else {
            for (auto callbackList : endContactCallbacks) {
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

    }

    void SetBeginContactCallback(int entityId, std::function<void(std::string)> callback) {
        if (beginContactCallbacks.find(entityId) != beginContactCallbacks.end()) {
            beginContactCallbacks[entityId].emplace_back(callback);
        } else {
            std::vector<std::function<void(std::string)>> callbacks;
            callbacks.emplace_back(callback);
            beginContactCallbacks.emplace(entityId, callbacks);
        }
    }

    void SetEndContactCallback(int entityId, std::function<void(std::string)> callback) {
        if (endContactCallbacks.find(entityId) != endContactCallbacks.end()) {
            endContactCallbacks[entityId].emplace_back(callback);
        } else {
            std::vector<std::function<void(std::string)>> callbacks;
            callbacks.emplace_back(callback);
            endContactCallbacks.emplace(entityId, callbacks);
        }
    }

    void Reset() {
        beginContactCallbacks.clear();
        endContactCallbacks.clear();
    }

private:
    std::map<int, std::vector<std::function<void(std::string)>>> beginContactCallbacks;
    std::map<int, std::vector<std::function<void(std::string)>>> endContactCallbacks;
};
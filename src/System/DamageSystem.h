#pragma once

#include "../ECS/ECS.h"

#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem: public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvent(const std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        if (event.a.BelongsToGroup("enemy") && event.b.BelongsToGroup("enemy")) {
            Logger::Log("The DamageSystem received an event between " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
            event.a.Kill();
            event.b.Kill();
        }
    }

    void Update() {

    }
};
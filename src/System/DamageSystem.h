#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem: public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Logger::Log("The DamageSystem received an event between " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
        event.a.Kill();
        event.b.Kill();
    }

    void Update() {

    }
};


#endif
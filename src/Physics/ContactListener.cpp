#include "./ContactListener.h"

#include "../Utils/CookyUtils.h"
#include "../ECS/ECS.h"
#include "../Events/CollisionEvent.h"

void ContactListener::BeginContact(b2Contact* contact) {
    Entity *e1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    Entity *e2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    e1->registry->entityEventBus->EmitEvent<CollisionEvent>(*e1, *e2);
}

void ContactListener::EndContact(b2Contact* contact) {
    // Logger::LogT("EndContact");
}
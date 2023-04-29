#include "./ContactListener.h"

#include "../Utils/CookyUtils.h"
#include "../ECS/ECS.h"
#include "../Events/CollisionEvent.h"

void ContactListener::BeginContact(b2Contact* contact) {
    b2WorldManifold* manifold = new b2WorldManifold;
    contact->GetWorldManifold(manifold);
    Vec2 points[2];
    int pointCount = contact->GetManifold()->pointCount;
    for (int i = 0; i < pointCount; i++) {
        points[i].Set(manifold->points[i].x, manifold->points[i].y);
    }
    Entity *e1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    Entity *e2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    e1->registry->entityEventBus->EmitEvent<CollisionEvent>(*e1, *e2, true, points, pointCount);
}

void ContactListener::EndContact(b2Contact* contact) {
    b2WorldManifold* manifold = new b2WorldManifold;
    contact->GetWorldManifold(manifold);
    Vec2 points[2];
    int pointCount = contact->GetManifold()->pointCount;
    for (int i = 0; i < pointCount; i++) {
        points[i].Set(manifold->points[i].x, manifold->points[i].y);
    }
    delete manifold;
    Entity *e1 = (Entity *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    Entity *e2 = (Entity *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    e1->registry->entityEventBus->EmitEvent<CollisionEvent>(*e1, *e2, false, points, pointCount);
}
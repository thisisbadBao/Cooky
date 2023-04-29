#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
    Entity a;
    Entity b;
    bool beginContact;
    Vec2* points;
    int pointCount;
    CollisionEvent(Entity a, Entity b, bool beginContact, Vec2* points, int pointCount)
    : a(a),
      b(b),
      beginContact(beginContact),
      points(points),
      pointCount(pointCount) {}
};
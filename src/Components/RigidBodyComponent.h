#pragma once

#include "../Utils/CookyUtils.h"
#include <box2d/box2d.h>

struct RigidBodyComponent {
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    Vec2 vel;
    bool isInit;

    RigidBodyComponent(Vec2 vel = Vec2(0.0, 0.0)) {
        this->vel = vel;
        this->isInit = false;
    }
};
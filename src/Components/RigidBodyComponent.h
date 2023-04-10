#pragma once

#include "../Utils/CookyUtils.h"
#include <box2d/box2d.h>

struct RigidBodyComponent {
    Vec2 vel;
    bool isInit;

    RigidBodyComponent(Vec2 vel = Vec2::Zero) {
        this->vel = vel;
        this->isInit = false;
    }

};
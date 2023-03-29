#pragma once

#include "../Utils/CookyUtils.h"

struct RigidBodyComponent {
    Vec2 velocity;

    RigidBodyComponent(Vec2 velocity = Vec2(0.0, 0.0)) {
        this->velocity = velocity;
    }
};
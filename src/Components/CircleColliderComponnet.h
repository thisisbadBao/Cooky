#pragma once

#include "../Utils/CookyUtils.h"
#include <box2d/box2d.h>

struct CircleColliderComponnet {
    Vec2 center;
    float radius;

    CircleColliderComponnet(Vec2 center = Vec2::Zero, float radius = 0) {
        this->center = center;
        this->radius = radius;
    }
};
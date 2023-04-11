#pragma once

#include "../Utils/CookyUtils.h"

struct TransformComponent {
    Vec2 position;
    Vec2 scale;
    float rotation;

    TransformComponent(Vec2 position = Vec2::Zero, Vec2 scale = Vec2(1,1), float rotation = 0.0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
};